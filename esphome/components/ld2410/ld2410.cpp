#include "ld2410.h"

#define highbyte(val) (uint8_t)((val) >> 8)
#define lowbyte(val) (uint8_t)((val)&0xff)

namespace esphome
{
  namespace ld2410
  {

    static const char *const TAG = "ld2410";

    void LD2410Component::dump_config()
    {
      ESP_LOGCONFIG(TAG, "LD2410:");
#ifdef USE_BINARY_SENSOR
      LOG_BINARY_SENSOR("  ", "HasTargetSensor", this->target_binary_sensor_);
      LOG_BINARY_SENSOR("  ", "MovingSensor", this->moving_binary_sensor_);
      LOG_BINARY_SENSOR("  ", "StillSensor", this->still_binary_sensor_);
#endif
#ifdef USE_SENSOR
      LOG_SENSOR("  ", "Moving Distance", this->moving_target_distance_sensor_);
      LOG_SENSOR("  ", "Still Distance", this->still_target_distance_sensor_);
      LOG_SENSOR("  ", "Moving Energy", this->moving_target_energy_sensor_);
      LOG_SENSOR("  ", "Still Energy", this->still_target_energy_sensor_);
      LOG_SENSOR("  ", "Detection Distance", this->detection_distance_sensor_);
#endif
      this->set_config_mode(true);
      this->get_version_();
      this->set_config_mode(false);
      ESP_LOGCONFIG(TAG, "  Firmware Version : %u.%u.%u%u%u%u", this->version_[0], this->version_[1], this->version_[2],
                    this->version_[3], this->version_[4], this->version_[5]);
    }

    void LD2410Component::setup() {}

    void LD2410Component::loop()
    {
      const int max_line_length = 80;
      static uint8_t buffer[max_line_length];

      while (available())
      {
        this->readline_(read(), buffer, max_line_length);
      }
    }

    void LD2410Component::send_command_(uint8_t command, uint8_t *command_value, int command_value_len)
    {
      // frame start bytes
      this->write_array(CMD_FRAME_HEADER, 4);
      // length bytes
      int len = 2;
      if (command_value != nullptr)
        len += command_value_len;
      this->write_byte(lowbyte(len));
      this->write_byte(highbyte(len));

      // command
      this->write_byte(lowbyte(command));
      this->write_byte(highbyte(command));

      // command value bytes
      if (command_value != nullptr)
      {
        for (int i = 0; i < command_value_len; i++)
        {
          this->write_byte(command_value[i]);
        }
      }
      // frame end bytes
      this->write_array(CMD_FRAME_END, 4);
      // FIXME to remove
      delay(50); // NOLINT
    }

    void LD2410Component::handle_periodic_data_(uint8_t *buffer, int len)
    {
      if (len < 12)
        return;                                                                             // 4 frame start bytes + 2 length bytes + 1 data end byte + 1 crc byte + 4 frame end bytes
      if (buffer[0] != 0xF4 || buffer[1] != 0xF3 || buffer[2] != 0xF2 || buffer[3] != 0xF1) // check 4 frame start bytes
        return;
      if (buffer[7] != HEAD || buffer[len - 6] != END || buffer[len - 5] != CHECK) // Check constant values
        return;                                                                    // data head=0xAA, data end=0x55, crc=0x00

        /*
          Data Type: 6th
          0x01: Engineering mode
          0x02: Normal mode
        */
        // char data_type = buffer[DATA_TYPES];
        /*
          Target states: 9th
          0x00 = No target
          0x01 = Moving targets
          0x02 = Still targets
          0x03 = Moving+Still targets
        */
#ifdef USE_BINARY_SENSOR
      char target_state = buffer[TARGET_STATES];
      if (this->target_binary_sensor_ != nullptr)
      {
        this->target_binary_sensor_->publish_state(target_state != 0x00);
      }
#endif

      /*
        Reduce data update rate to prevent home assistant database size grow fast
      */
      int32_t current_millis = millis();
      if (current_millis - last_periodic_millis < 1000)
        return;
      last_periodic_millis = current_millis;

#ifdef USE_BINARY_SENSOR
      if (this->moving_binary_sensor_ != nullptr)
      {
        this->moving_binary_sensor_->publish_state(CHECK_BIT(target_state, 0));
      }
      if (this->still_binary_sensor_ != nullptr)
      {
        this->still_binary_sensor_->publish_state(CHECK_BIT(target_state, 1));
      }
#endif
      /*
        Moving target distance: 10~11th bytes
        Moving target energy: 12th byte
        Still target distance: 13~14th bytes
        Still target energy: 15th byte
        Detect distance: 16~17th bytes
      */
#ifdef USE_SENSOR
      if (this->moving_target_distance_sensor_ != nullptr)
      {
        int new_moving_target_distance = this->two_byte_to_int_(buffer[MOVING_TARGET_LOW], buffer[MOVING_TARGET_HIGH]);
        if (this->moving_target_distance_sensor_->get_state() != new_moving_target_distance)
          this->moving_target_distance_sensor_->publish_state(new_moving_target_distance);
      }
      if (this->moving_target_energy_sensor_ != nullptr)
      {
        int new_moving_target_energy = buffer[MOVING_ENERGY];
        if (this->moving_target_energy_sensor_->get_state() != new_moving_target_energy)
          this->moving_target_energy_sensor_->publish_state(new_moving_target_energy);
      }
      if (this->still_target_distance_sensor_ != nullptr)
      {
        int new_still_target_distance = this->two_byte_to_int_(buffer[STILL_TARGET_LOW], buffer[STILL_TARGET_HIGH]);
        if (this->still_target_distance_sensor_->get_state() != new_still_target_distance)
          this->still_target_distance_sensor_->publish_state(new_still_target_distance);
      }
      if (this->still_target_energy_sensor_ != nullptr)
      {
        int new_still_target_energy = buffer[STILL_ENERGY];
        if (this->still_target_energy_sensor_->get_state() != new_still_target_energy)
          this->still_target_energy_sensor_->publish_state(new_still_target_energy);
      }
      if (this->detection_distance_sensor_ != nullptr)
      {
        int new_detect_distance = this->two_byte_to_int_(buffer[DETECT_DISTANCE_LOW], buffer[DETECT_DISTANCE_HIGH]);
        if (this->detection_distance_sensor_->get_state() != new_detect_distance)
          this->detection_distance_sensor_->publish_state(new_detect_distance);
      }
#endif
    }

    void LD2410Component::handle_ack_data_(uint8_t *buffer, int len)
    {
      ESP_LOGV(TAG, "Handling ACK DATA for COMMAND");
      if (len < 10)
      {
        ESP_LOGE(TAG, "Error with last command : incorrect length");
        return;
      }
      if (buffer[0] != 0xFD || buffer[1] != 0xFC || buffer[2] != 0xFB || buffer[3] != 0xFA)
      { // check 4 frame start bytes
        ESP_LOGE(TAG, "Error with last command : incorrect Header");
        return;
      }
      if (buffer[COMMAND_STATUS] != 0x01)
      {
        ESP_LOGE(TAG, "Error with last command : status != 0x01");
        return;
      }
      if (this->two_byte_to_int_(buffer[8], buffer[9]) != 0x00)
      {
        ESP_LOGE(TAG, "Error with last command , last buffer was: %u , %u", buffer[8], buffer[9]);
        return;
      }
      switch (buffer[COMMAND])
      {
      case lowbyte(CMD_ENABLE_CONF):
        ESP_LOGV(TAG, "Handled Enable conf command");
        break;
      case lowbyte(CMD_DISABLE_CONF):
        ESP_LOGV(TAG, "Handled Disabled conf command");
        break;
      case lowbyte(CMD_VERSION):
        ESP_LOGV(TAG, "FW Version is: %u.%u.%u%u%u%u", buffer[13], buffer[12], buffer[17], buffer[16], buffer[15],
                 buffer[14]);
        this->version_[0] = buffer[13];
        this->version_[1] = buffer[12];
        this->version_[2] = buffer[17];
        this->version_[3] = buffer[16];
        this->version_[4] = buffer[15];
        this->version_[5] = buffer[14];
        break;
      case lowbyte(CMD_GATE_SENS):
        ESP_LOGV(TAG, "Handled sensitivity command");
        break;
      case lowbyte(CMD_QUERY): // Query parameters response
      {
        if (buffer[10] != 0xAA)
          return; // value head=0xAA

        /*
          Moving distance range: 13th byte
          Still distance range: 14th byte
        */
        this->moving_distance_range_->publish_state(buffer[12] * 0.75);

        /*
          Moving Sensitivities: 15~23th bytes
          Still Sensitivities: 24~32th bytes
        */
        for (int i = 0; i < 9; i++)
        {
          moving_sensitivities[i] = buffer[14 + i];
        }
        for (int i = 0; i < 9; i++)
        {
          still_sensitivities[i] = buffer[23 + i];
        }
        this->moving_sensitivity_threshold_->publish_state(moving_sensitivities[4]);
        this->still_sensitivity_threshold_->publish_state(still_sensitivities[4]);

        /*
          None Duration: 33~34th bytes
        */
        this->none_duration_->publish_state(this->two_byte_to_int_(buffer[32], buffer[33]));
      }
      break;
      default:
        break;
      }
    }

    void LD2410Component::readline_(int readch, uint8_t *buffer, int len)
    {
      static int pos = 0;

      if (readch >= 0)
      {
        if (pos < len - 1)
        {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
        else
        {
          pos = 0;
        }
        if (pos >= 4)
        {
          if (buffer[pos - 4] == 0xF8 && buffer[pos - 3] == 0xF7 && buffer[pos - 2] == 0xF6 && buffer[pos - 1] == 0xF5)
          {
            ESP_LOGV(TAG, "Will handle Periodic Data");
            this->handle_periodic_data_(buffer, pos);
            pos = 0; // Reset position index ready for next time
          }
          else if (buffer[pos - 4] == 0x04 && buffer[pos - 3] == 0x03 && buffer[pos - 2] == 0x02 &&
                   buffer[pos - 1] == 0x01)
          {
            ESP_LOGV(TAG, "Will handle ACK Data");
            this->handle_ack_data_(buffer, pos);
            pos = 0; // Reset position index ready for next time
          }
        }
      }
    }

    void LD2410Component::set_config_mode(bool enable)
    {
      uint8_t cmd = enable ? CMD_ENABLE_CONF : CMD_DISABLE_CONF;
      uint8_t cmd_value[2] = {0x01, 0x00};
      this->send_command_(cmd, enable ? cmd_value : nullptr, 2);
    }

    void LD2410Component::query_parameters() { this->send_command_(CMD_QUERY, nullptr, 0); }

    void LD2410Component::get_version_() { this->send_command_(CMD_VERSION, nullptr, 0); }

    void LD2410Component::set_max_distances_timeout(int max_distance_range, int timeout)
    {
      uint8_t value[18] = {0x00,
                           0x00,
                           lowbyte(max_distance_range),
                           highbyte(max_distance_range),
                           0x00,
                           0x00,
                           0x01,
                           0x00,
                           lowbyte(max_distance_range),
                           highbyte(max_distance_range),
                           0x00,
                           0x00,
                           0x02,
                           0x00,
                           lowbyte(timeout),
                           highbyte(timeout),
                           0x00,
                           0x00};
      this->send_command_(CMD_MAXDIST_DURATION, value, 18);
      this->query_parameters();
    }

    void LD2410Component::set_thresholds(int moving_sens, int still_sens)
    {
      // reference
      // https://drive.google.com/drive/folders/1p4dhbEJA3YubyIjIIC7wwVsSo8x29Fq-?spm=a2g0o.detail.1000023.17.93465697yFwVxH
      //   Send data: configure the motion sensitivity of distance gate 3 to 40, and the static sensitivity of 40
      // 00 00 (gate)
      // 03 00 00 00 (gate number)
      // 01 00 (motion sensitivity)
      // 28 00 00 00 (value)
      // 02 00 (still sensitivtiy)
      // 28 00 00 00 (value)
      uint8_t value[18] = {0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
                           0x01, 0x00, lowbyte(moving_sens), highbyte(moving_sens), 0x00, 0x00,
                           0x02, 0x00, lowbyte(still_sens), highbyte(still_sens), 0x00, 0x00};
      this->send_command_(CMD_GATE_SENS, value, 18);

      // Overrides for gates 0-1 to counter excessive fluctuations in the sensor
      int max_gate_value = 100;
      int moving_gate_value_additions[2] = {20, 20};
      for (int i = 0; i < 2; i++){
        int moving_gate_value = std::min(moving_sens + moving_gate_value_additions[i], max_gate_value);
        uint8_t override_value[18] = {0x00, 0x00, lowbyte(i), highbyte(i), 0x00, 0x00,
                     0x01, 0x00, lowbyte(moving_gate_value), highbyte(moving_gate_value), 0x00, 0x00,
                     0x02, 0x00, lowbyte(still_sens),  highbyte(still_sens),  0x00, 0x00};    
        this->send_command_(CMD_GATE_SENS, override_value, 18);
      }
    }

    void LD2410Component::factory_reset() { this->send_command_(CMD_FACTORY_RESET, nullptr, 0); }

    void LD2410Component::reboot() { this->send_command_(CMD_REBOOT, nullptr, 0); }

  } // namespace ld2410
} // namespace esphome