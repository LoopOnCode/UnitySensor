#pragma once
#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#include "esphome/components/number/number.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

namespace esphome
{
  namespace ld2410
  {

#define CHECK_BIT(var, pos) (((var) >> (pos)) & 1)

    // Commands
    static const uint8_t CMD_ENABLE_CONF = 0x00FF;
    static const uint8_t CMD_DISABLE_CONF = 0x00FE;
    static const uint8_t CMD_MAXDIST_DURATION = 0x0060;
    static const uint8_t CMD_QUERY = 0x0061;
    static const uint8_t CMD_GATE_SENS = 0x0064;
    static const uint8_t CMD_VERSION = 0x00A0;
    static const uint8_t CMD_FACTORY_RESET = 0x00A2;
    static const uint8_t CMD_REBOOT = 0x00A3;

    // Commands values
    static const uint8_t CMD_MAX_MOVE_VALUE = 0x0000;
    static const uint8_t CMD_MAX_STILL_VALUE = 0x0001;
    static const uint8_t CMD_DURATION_VALUE = 0x0002;
    // Command Header & Footer
    static const uint8_t CMD_FRAME_HEADER[4] = {0xFD, 0xFC, 0xFB, 0xFA};
    static const uint8_t CMD_FRAME_END[4] = {0x04, 0x03, 0x02, 0x01};
    // Data Header & Footer
    static const uint8_t DATA_FRAME_HEADER[4] = {0xF4, 0xF3, 0xF2, 0xF1};
    static const uint8_t DATA_FRAME_END[4] = {0xF8, 0xF7, 0xF6, 0xF5};
    /*
    Data Type: 6th byte
    Target states: 9th byte
        Moving target distance: 10~11th bytes
        Moving target energy: 12th byte
        Still target distance: 13~14th bytes
        Still target energy: 15th byte
        Detect distance: 16~17th bytes
    */
    enum PeriodicDataStructure : uint8_t
    {
      DATA_TYPES = 5,
      TARGET_STATES = 8,
      MOVING_TARGET_LOW = 9,
      MOVING_TARGET_HIGH = 10,
      MOVING_ENERGY = 11,
      STILL_TARGET_LOW = 12,
      STILL_TARGET_HIGH = 13,
      STILL_ENERGY = 14,
      DETECT_DISTANCE_LOW = 15,
      DETECT_DISTANCE_HIGH = 16,
    };
    enum PeriodicDataValue : uint8_t
    {
      HEAD = 0XAA,
      END = 0x55,
      CHECK = 0x00
    };

    enum AckDataStructure : uint8_t
    {
      COMMAND = 6,
      COMMAND_STATUS = 7
    };

    //  char cmd[2] = {enable ? 0xFF : 0xFE, 0x00};
    class LD2410Component : public Component, public uart::UARTDevice
    {
#ifdef USE_SENSOR
      SUB_SENSOR(moving_target_distance)
      SUB_SENSOR(still_target_distance)
      SUB_SENSOR(moving_target_energy)
      SUB_SENSOR(still_target_energy)
      SUB_SENSOR(detection_distance)
#endif

    public:
      void setup() override;
      void dump_config() override;
      void loop() override;

#ifdef USE_BINARY_SENSOR
      void set_target_sensor(binary_sensor::BinarySensor *sens)
      {
        this->target_binary_sensor_ = sens;
      };
      void set_moving_target_sensor(binary_sensor::BinarySensor *sens) { this->moving_binary_sensor_ = sens; };
      void set_still_target_sensor(binary_sensor::BinarySensor *sens) { this->still_binary_sensor_ = sens; };
#endif

      int moving_sensitivities[9] = {0};
      int still_sensitivities[9] = {0};

      int32_t last_periodic_millis = millis();

      void set_config_mode(bool enable);
      void query_parameters();
      void set_thresholds(int moving_sens, int still_sens);
      void set_max_distances_timeout(int max_distance_range, int timeout);
      void set_numbers(number::Number *moving_distance_range, number::Number *none_duration, number::Number *moving_sensitivity_threshold, number::Number *still_sensitivity_threshold)
      {
        this->moving_distance_range_ = moving_distance_range;
        this->none_duration_ = none_duration;
        this->moving_sensitivity_threshold_ = moving_sensitivity_threshold;
        this->still_sensitivity_threshold_ = still_sensitivity_threshold;
      }
      void factory_reset();
      void reboot();

    protected:
#ifdef USE_BINARY_SENSOR
      binary_sensor::BinarySensor *target_binary_sensor_{nullptr};
      binary_sensor::BinarySensor *moving_binary_sensor_{nullptr};
      binary_sensor::BinarySensor *still_binary_sensor_{nullptr};
#endif

      std::vector<uint8_t> rx_buffer_;
      int two_byte_to_int_(char firstbyte, char secondbyte) { return (int16_t)(secondbyte << 8) + firstbyte; }
      void send_command_(uint8_t command_str, uint8_t *command_value, int command_value_len);

      void handle_periodic_data_(uint8_t *buffer, int len);
      void handle_ack_data_(uint8_t *buffer, int len);
      void readline_(int readch, uint8_t *buffer, int len);
      void get_version_();

      number::Number *moving_distance_range_;
      number::Number *none_duration_;
      number::Number *moving_sensitivity_threshold_;
      number::Number *still_sensitivity_threshold_;

      uint8_t version_[6];
    };

  } // namespace ld2410
} // namespace esphome