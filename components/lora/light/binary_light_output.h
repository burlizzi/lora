#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/remote_base/rc_switch_protocol.h"
#include "esphome/components/remote_base/remote_base.h"
//#include "esphome/components/rfm12_transmitter/remote_transmitter.h"
//#include "esphome/components/remote_receiver/remote_receiver.h"
#define PULSE 424
namespace esphome {
namespace binary {

static const char *const TAG = "loralight";



class RfmLightOutput : public light::LightOutput {
 public:
  RfmLightOutput()
  {
    action=new remote_base::RCSwitchTypeAAction<bool>();
  }
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::ON_OFF});
    return traits;
  }
  void set_type_a(const char* group,const char* device)
  {
    action->set_group(group);
    action->set_device(device);
    action->set_protocol(remote_base::RC_SWITCH_PROTOCOLS[1]);
  }

  void set_transmitter(remote_base::RemoteTransmitterBase *transmitter) {
    action->set_transmitter(transmitter);
  }

  void write_state(light::LightState *state) override {
    bool binary;
    state->current_values_as_binary(&binary);
    action->set_send_times(10);
    action->set_send_wait(0);
    action->play_complex(binary);
  }

 protected:
 remote_base::RCSwitchTypeAAction<bool>* action;
};

}  // namespace binary
}  // namespace esphome
