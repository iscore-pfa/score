#pragma once

#include "CoherencyCheckerFactoryInterface.hpp"

#include <score/plugins/customfactory/FactoryFamily.hpp>
#include <score/plugins/customfactory/FactoryInterface.hpp>

namespace Scenario
{
class SCORE_PLUGIN_SCENARIO_EXPORT CSPCoherencyCheckerList final
    : public score::InterfaceList<CoherencyCheckerFactoryInterface>
{
public:
  CoherencyCheckerFactoryInterface* get() const;
};
}
