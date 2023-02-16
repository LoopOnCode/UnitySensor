# import esphome.codegen as cg
# from esphome.components import number
# import esphome.config_validation as cv
# from esphome.const import (
#     DEVICE_CLASS_DISTANCE,
#     DEVICE_CLASS_ENERGY,
#     UNIT_CENTIMETER,
#     UNIT_PERCENT,
#     UNIT_SECOND,
#     ICON_TIMER,
#     CONF_TIMEOUT,
#     CONF_NAME,
#     CONF_TYPE,
#     CONF_MIN_VALUE,
#     CONF_MAX_VALUE,
#     CONF_STEP,
#     CONF_ID
# )
# from . import CONF_LD2410_ID, LD2410Component

# DEPENDENCIES = ["ld2410"]

# CONF_NUMBERS = "numbers"

# # CONFIG_SCHEMA = cv.Schema(
# #         {
# #             cv.Required(CONF_TIMEOUT): cv.All(
# #                         cv.positive_time_period_seconds,
# #                         cv.Range(max=cv.TimePeriod(seconds=32767)),
# #             ),
# #         }).extend(cv.COMPONENT_SCHEMA)

# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.Optional(
#             CONF_NUMBERS,
#             default=[
#                 {
#                     CONF_NAME: "Demo Number 0-100",
#                     CONF_TYPE: 1,
#                     CONF_MIN_VALUE: 0,
#                     CONF_MAX_VALUE: 100,
#                     CONF_STEP: 1,
#                 },
#                 # {
#                 #     CONF_NAME: "Demo Number -50-50",
#                 #     CONF_MIN_VALUE: -50,
#                 #     CONF_MAX_VALUE: 50,
#                 #     CONF_STEP: 5,
#                 # },
#                 # {
#                 #     CONF_NAME: "Demo Number 40-60",
#                 #     CONF_MIN_VALUE: 40,
#                 #     CONF_MAX_VALUE: 60,
#                 #     CONF_STEP: 0.2,
#                 # },
#             ],
#         ): [
#             number.NUMBER_SCHEMA.extend(cv.COMPONENT_SCHEMA).extend(
#                 {
#                     cv.GenerateID(CONF_LD2410_ID): cv.use_id(LD2410Component),
#                     cv.Required(CONF_MIN_VALUE): cv.float_,
#                     cv.Required(CONF_MAX_VALUE): cv.float_,
#                     cv.Required(CONF_STEP): cv.float_,
#                 }
#             )
#         ]
#     }
# )

# async def to_code(config):
#     # ld2410_component = await cg.get_variable(config[CONF_LD2410_ID])
#     # var = cg.new_Pvariable(config[CONF_ID])
#     # var = cg.new_Pvariable(config[CONF_LD2410_ID])
#     # await cg.register_component(var, config)
#     # cg.add(var.set_keep_alive(config[CONF_TIMEOUT]))

#     # var = cg.new_Pvariable(config[CONF_LD2410_ID])
#     # await cg.register_component(var, config)
#     # await number.register_number(
#     #     var,
#     #     config,
#     #     min_value=0,
#     #     max_value=900,
#     #     step=1,
#     # )

#     for conf in config[CONF_NUMBERS]:
#         var = cg.new_Pvariable(conf[CONF_ID])
#         await cg.register_component(var, conf)
#         await number.register_number(
#             var,
#             conf,
#             min_value=conf[CONF_MIN_VALUE],
#             max_value=conf[CONF_MAX_VALUE],
#             step=conf[CONF_STEP],
#         )
#         cg.add(var.set_type(conf[CONF_TYPE]))
