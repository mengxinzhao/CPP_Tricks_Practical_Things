//
// Created by Vesper on 6/8/18.
//

#include "config.h"
#include "config_builder.h"
ConfigBuilder Config :: create() {
    return ConfigBuilder{ std::make_shared<Config>(Config {})};
}