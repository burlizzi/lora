#pragma once
#include "esphome.h"
#include "esphome/core/controller.h"
#include "esphome/core/component.h"
#include "esphome/core/preferences.h"
#include "esphome/core/automation.h"
#include <RadioLib.h>



namespace esphome{
    namespace lora{


        class Lora : public Component
        {
            public:
            

            Lora();
            void setDevEUI(uint64_t eui){}
            void setAppEUI(uint64_t eui){}
            void setAppKey(std::array<uint8_t,16> eui){}
            void setDeviceClass(int lora_class){}

            virtual void setup() override
            {
                //setup_controller();
                //uint32_t* dev=(uint32_t*)DevEUI;
                //rtc = global_preferences->make_preference<sLoRa_Session>(RESTORE_STATE_VERSION^dev[0]^dev[1],true);
            }

           

            void loop() override
            {
                char outStr[255];
                //update();
                //auto recvStatus = readData(outStr);
                //if(recvStatus) {
                //    ESP_LOGI("lora","====>>(%d) rssi(%d) %s",Message_Rx.Frame_Port, getRssi(), outStr);
                //    if(data_trigger_)
                //        data_trigger_->trigger(Message_Rx.Frame_Port,getRssi(),std::string(outStr,recvStatus));
                //}
            }

            Trigger<int,int,std::string> *data_trigger() {
                if (!this->data_trigger_)
                this->data_trigger_ = make_unique<Trigger<int,int,std::string>>();
                return this->data_trigger_.get();
            }
            protected:
            std::unique_ptr<Trigger<int,int,std::string>> data_trigger_{nullptr};
            static const uint32_t RESTORE_STATE_VERSION = 0x2a5a2c13UL;
            ESPPreferenceObject rtc;

        };


    }
}
