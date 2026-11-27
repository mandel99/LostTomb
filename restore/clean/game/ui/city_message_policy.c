#include "pharaoh/ui/city_message_policy.h"

uint32_t pharaoh_city_message_should_popup(
    const PharaohCityMessagePolicyContext *c,int32_t message,
    const int16_t *parameters)
{
  uint32_t result;

  result = 1;
  if (message == 0x84) {
    return result;
  }
  if (message == 0x15d) {
    if (((*c->message_options) & 2) != 0) {
      pharaoh_city_warning_post(c->warnings,0xdb);
      return 0;
    }
  }
  else if (message == 0x15c) {
    if (((*c->message_options) & 2) != 0) {
      pharaoh_city_warning_post(c->warnings,0xdc);
      return 0;
    }
  }
  else if (message == 0x15b) {
    if (((*c->message_options) & 2) != 0) {
      pharaoh_city_warning_post(c->warnings,0xdd);
      return 0;
    }
  }
  else {
    if (message != 0x156) {
      if (message != 0x15a) {
        if (message != 0x157) {
          if (message != 0x159) {
            if (message == 0x158) {
              if (((*c->message_options) & 2) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe0);
              return 0;
            }
            if (message == 0x115) {
              if (((*c->message_options) & 0x80) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe1);
              return 0;
            }
            if (message == 0x116) {
              if (((*c->message_options) & 0x80) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe2);
              return 0;
            }
            if (message == 0x10c) {
              if (((*c->message_options) & 0x200) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe3);
              return 0;
            }
            if (message == 0x10b) {
              if (((*c->message_options) & 0x200) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe4);
              return 0;
            }
            if (message == 0x112) {
              if (((*c->message_options) & 0x100) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe5);
              return 0;
            }
            if (message == 0x111) {
              if (((*c->message_options) & 0x100) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe6);
              return 0;
            }
            if (message == 0x83) {
              if (*parameters != 0xc0) {
                return 1;
              }
              if (((*c->message_options) & 0x10) == 0) {
                return 1;
              }
              pharaoh_city_warning_post(c->warnings,0xe7);
              return 0;
            }
            if (((99 < message) && (message < 0x6e)) && (((*c->message_options) & 4) != 0)) {
              pharaoh_city_warning_post(c->warnings,0xe8);
              return 0;
            }
            if ((((0x140 < message) && (message < 0x146)) || (message == 0x14b)) &&
               (((*c->message_options) & 0x40) != 0)) {
              pharaoh_city_warning_post(c->warnings,0xe9);
              return 0;
            }
            if (((0x88 < message) && (message < 0x8c)) && (((*c->message_options) & 0x20) != 0)) {
              pharaoh_city_warning_post(c->warnings,0xea);
              return 0;
            }
            if (message != 0x135) {
              if (message != 0x13a) {
                if (message == 0x146) {
                  if (((*c->message_options) & 0x400) == 0) {
                    return 1;
                  }
                  pharaoh_city_warning_post(c->warnings,0xf6);
                  return 0;
                }
                if (message == 0x148) {
                  if (((*c->message_options) & 0x800) == 0) {
                    return 1;
                  }
                  pharaoh_city_warning_post(c->warnings,0xf7);
                  return 0;
                }
                if (message != 0x88) {
                  return 1;
                }
                if (((*c->message_options) & 0x1000) == 0) {
                  return 1;
                }
                pharaoh_city_warning_post(c->warnings,0x10f);
                return 0;
              }
              if (((*c->message_options) & 8) == 0) {
                return result;
              }
            }
            pharaoh_city_warning_post(c->warnings,0xeb);
            pharaoh_scenario_request_dispatch(c->requests,(*c->current_request));
            return 0;
          }
          if (((*c->message_options) & 2) == 0) {
            return result;
          }
        }
        pharaoh_city_warning_post(c->warnings,0xdf);
        return 0;
      }
      if (((*c->message_options) & 2) == 0) {
        return result;
      }
    }
    pharaoh_city_warning_post(c->warnings,0xde);
    result = 0;
  }
  return result;
}
