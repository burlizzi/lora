import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output, remote_base, remote_receiver
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT, CONF_LIGHT_ID
from esphome.components.remote_base import CONF_RECEIVER_ID, CONF_TRANSMITTER_ID

from .. import Lora
from esphome.const import (
    CONF_GROUP,
    CONF_DEVICE,
    CONF_ON,
)
CONF_OFF = "off"
AUTO_LOAD = ["output"]

binary_ns = cg.esphome_ns.namespace("binary")

BinaryLightOutput = binary_ns.class_("RfmLightOutput", light.LightOutput)

CONFIG_SCHEMA = light.BINARY_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_TRANSMITTER_ID): cv.use_id(
            Lora
        ),
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(BinaryLightOutput),
        cv.Optional(CONF_GROUP): cv.All(
            remote_base.validate_rc_switch_code, cv.Length(min=5, max=5)
        ),
        cv.Optional(CONF_DEVICE): cv.All(
            remote_base.validate_rc_switch_code, cv.Length(min=5, max=5)
        ),
        cv.Optional(CONF_ON): cv.All(
            cv.hex_uint32_t
        ),
        cv.Optional(CONF_OFF): cv.All(
            cv.hex_uint32_t
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    if CONF_GROUP in config and CONF_DEVICE in config: 
        cg.add(var.set_type_a(config[CONF_GROUP],config[CONF_DEVICE]))
    elif CONF_ON in config and CONF_OFF in config: 
        cg.add(var.set_on_off(config[CONF_ON],config[CONF_OFF]))
    transmitter = await cg.get_variable(config[CONF_TRANSMITTER_ID])
    cg.add(var.set_transmitter(transmitter))
 