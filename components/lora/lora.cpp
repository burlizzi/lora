#include "lora.h"

void xxx(const char *format, ...)
{ // NOLINT
  va_list arg;
  va_start(arg, format);

  esphome::esp_log_vprintf_(ESPHOME_LOG_LEVEL_ERROR, "lora", 22, format, arg);
  va_end(arg);
}

namespace esphome
{
  namespace lora
  {

    Lora::Lora(esphome::InternalGPIOPin* pin) : joined(false) //remote_base::RemoteTransmitterBase(pin) ,
    {
      pinMode(MISO, INPUT_PULLUP);
      pinMode(0, INPUT_PULLDOWN);
      if (!init())
      {
        ESP_LOGE("lora", "RFM95 not detected");
      }
    }

/*
void Lora::send_internal(uint32_t send_times, uint32_t send_wait) {
  std::stringstream s;
  for (int32_t item : this->temp_.get_data()) 
    s<<item<<',';


  ESP_LOGD("TAG", "Sending ook remote code... %d %d %d %s",send_wait,send_times,this->temp_.get_data().size(),s.str().c_str());
  

  SX1276 radio = new Module(RFM_pins.CS, RFM_pins.DIO0, RFM_pins.RST, RFM_pins.DIO1);

  int state = radio.beginFSK(433.97);
  if (state == RADIOLIB_ERR_NONE) {
    xxx("beginFSK success!");
  } else {
    xxx("Error=%d", state);
  }

  state = radio.setOutputPower(20.0);
  if (state == RADIOLIB_ERR_NONE) {
    xxx("setOutputPower success!");
  } else {
    xxx("Error=%d", state);
  }
  state = radio.setOOK(true);

  if (state == RADIOLIB_ERR_NONE) {
    xxx("setOOK success!");
  } else {
    xxx("Error=%d", state);
  }

  state = radio.transmitDirect();
  if (state == RADIOLIB_ERR_NONE) {
    xxx("transmitDirect success!");
  } else {
    xxx("Error=%d", state);
  }

  pinMode(25, OUTPUT);


  for (uint32_t i = 0; i < send_times; i++) {
    {
      InterruptLock lock;
      for (int32_t item : this->temp_.get_data()) {
        if (item > 0) {
          const auto length = uint32_t(item);
          digitalWrite(25, 1);
          delay_microseconds_safe(length);
          digitalWrite(25, 0);
        } else {
          const auto length = uint32_t(-item);
          delay_microseconds_safe(length);
        }
        App.feed_wdt();
      }
    }
    if (i + 1 < send_times) {
      delay_microseconds_safe(send_wait);
    }
  }



}
    */
    void Lora::ook(uint64_t code, unsigned char len)
    {
      SX1276 radio = new Module(RFM_pins.CS, RFM_pins.DIO0, RFM_pins.RST, RFM_pins.DIO1);

      int state = radio.beginFSK(433.97);
      if (state == RADIOLIB_ERR_NONE) {
        xxx("beginFSK success!");
      } else {
        xxx("Error=%d", state);
      }

      state = radio.setOutputPower(20.0);
      if (state == RADIOLIB_ERR_NONE) {
        xxx("setOutputPower success!");
      } else {
        xxx("Error=%d", state);
      }
      state = radio.setOOK(true);

      if (state == RADIOLIB_ERR_NONE) {
        xxx("setOOK success!");
      } else {
        xxx("Error=%d", state);
      }

      state = radio.transmitDirect();
      if (state == RADIOLIB_ERR_NONE) {
        xxx("transmitDirect success!");
      } else {
        xxx("Error=%d", state);
      }

      pinMode(25, OUTPUT);
      for (size_t i = 0; i < 6; i++)
      {
        delayMicroseconds(12000);
        digitalWrite(25, 1);
        delayMicroseconds(550);
        digitalWrite(25, 0);
        delayMicroseconds(1200);
        // std::ostringstream ss;
        for (size_t bit = 0; bit < len; bit++)
        {
          int x = code >> bit & 1;
          // ss<<x;
          //    xxx("%d",x);
          digitalWrite(25, 1);
          delayMicroseconds(x ? 1200 : 550);
          digitalWrite(25, 0);
          delayMicroseconds(x ? 550 : 1200);
        }
        // xxx(ss.str().c_str());
      }
      pinMode(25, INPUT);
          state = radio.standby();

      if (state == RADIOLIB_ERR_NONE) {
        xxx("success!");
      } else {
        xxx("Error=%d", state);
      }

      
      
      RFM_Init();
    setDeviceClass(CLASS_C);

    // Set Data Rate
    setDataRate(drate_common);

    // set channel to random
    setChannel(MULTI);
    }

  }
}