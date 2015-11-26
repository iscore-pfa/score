#include "LoopTriggerCommandFactory.hpp"
#include <Loop/LoopProcessModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Commands/TimeNode/AddTrigger.hpp>
#include <Scenario/Commands/TimeNode/RemoveTrigger.hpp>

bool LoopTriggerCommandFactory::matches(const TimeNodeModel& tn) const
{
  return dynamic_cast<Loop::ProcessModel*>(tn.parentScenario());
}

iscore::SerializableCommand* LoopTriggerCommandFactory::make_addTriggerCommand(const TimeNodeModel& tn) const
{
  if(dynamic_cast<Loop::ProcessModel*>(tn.parentScenario()))
  {
    return new Scenario::Command::AddTrigger<Loop::ProcessModel>(tn);
  }
  return nullptr;
}

iscore::SerializableCommand* LoopTriggerCommandFactory::make_removeTriggerCommand(const TimeNodeModel& tn) const
{
  if(dynamic_cast<Loop::ProcessModel*>(tn.parentScenario()))
  {
    return new Scenario::Command::RemoveTrigger<Loop::ProcessModel>(tn);
  }
  return nullptr;
}
