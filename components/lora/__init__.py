
from typing import Literal
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome import automation
from esphome.const import  CONF_ID, CONF_CS_PIN
from esphome.core import CORE

CONF_BAND = "band"
CONF_DEVEUI = "deveui"
CONF_APPEUI = "appeui"
CONF_APPKEY = "appkey"
CONF_RESET = "rst_pin"
CONF_DIO0 = "dio0_pin"
CONF_DIO1 = "dio1_pin"
CONF_DIO2 = "dio2_pin"
CONF_DIO5 = "dio5_pin"
CONF_DATA_ACTION = "data_action"
CONF_LORA_CLASS = "class"
CONF_LORA_FREQ = "FREQUENCY_PLAN"


DEPENDENCIES = ["spi"]


ns = lora_ns = cg.esphome_ns.namespace("lora")
Lora = ns.class_("Lora",cg.Component)




RX2DR_ENUM = cg.global_ns.enum("dataRates_t")

RX2DR = {
    'SF9': RX2DR_ENUM.SF9BW125,
    'SF12': RX2DR_ENUM.SF12BW125,
}





#MULTI_CONF = True
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Lora),

        cv.Required(CONF_BAND): cv.one_of('EU868','US915','EU433','AU915','CN500','AS923','AS923_2','AS923_3','AS923_4','KR920','IN865'),
        cv.Required(CONF_DEVEUI): cv.string,
        cv.Optional(CONF_APPEUI,"0000000000000000"): cv.string,
        cv.Required(CONF_APPKEY): cv.string,

        cv.Optional(CONF_LORA_CLASS,'CLASS_A'): cv.one_of('CLASS_A','CLASS_C'),
        cv.Optional(CONF_LORA_FREQ,'SF12'): cv.enum(RX2DR),


        cv.Optional(CONF_CS_PIN,'SS'): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_RESET,'RST_LoRa'): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_DIO0,'DIO0'): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_DIO1,'DIO1'): pins.internal_gpio_input_pin_number,
        cv.Optional(CONF_DIO2,'DIO2'): pins.internal_gpio_input_pin_number,
        cv.Optional(CONF_DIO5): pins.internal_gpio_input_pin_number,

        cv.Optional(CONF_DATA_ACTION): automation.validate_automation(
            single=True
        ),        
        
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    if CORE.using_arduino:
        cg.add_library("SPI", "")
        cg.add_platformio_option("lib_ldf_mode", "chain+")


    cg.add_library("RadioLib", "")
    #cg.add_build_flag("-D"+config[CONF_BAND])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.setDevEUI(int(config[CONF_DEVEUI],16)))
    cg.add(var.setAppEUI(int(config[CONF_APPEUI],16)))
    app_key=[]
    for i in range(0, len(config[CONF_APPKEY]), 2):
        couple=config[CONF_APPKEY][i:i+2]
        n=int(couple,16)
        app_key.append(n)
    cg.add(var.setAppKey(app_key))
    #cg.add(var.setDeviceClass('RADIOLIB_LORAWAN_'+config[CONF_LORA_CLASS]))
    #cg.add(var.setRX2DR(config[CONF_LORA_FREQ]))
    
    if CONF_DATA_ACTION in config:
        await automation.build_automation(
            var.data_trigger(), [(int, "port"),(int, "rssi"),(cg.std_string, "data")], config[CONF_DATA_ACTION]
        )
    #cg.add_global(cg.RawStatement(f'const sRFM_pins RFM_pins = {{.CS={config[CONF_CS_PIN]},.RST={config[CONF_RESET]},.DIO0={config[CONF_DIO0]},.DIO1={config[CONF_DIO1]},.DIO2={config[CONF_DIO2]}}};'))
