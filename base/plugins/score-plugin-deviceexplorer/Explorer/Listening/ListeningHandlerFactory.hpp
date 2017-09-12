#pragma once
#include <Explorer/Listening/ListeningHandler.hpp>
#include <score/plugins/customfactory/FactoryInterface.hpp>
#include <score_plugin_deviceexplorer_export.h>
namespace score
{
struct DocumentContext;
}
namespace Explorer
{
class DeviceDocumentPlugin;
class SCORE_PLUGIN_DEVICEEXPLORER_EXPORT ListeningHandlerFactory
    : public score::Interface<ListeningHandlerFactory>
{
  SCORE_INTERFACE("42828393-b8de-45a6-b79f-811eea2e1a40")

public:
  virtual ~ListeningHandlerFactory();

  virtual std::unique_ptr<Explorer::ListeningHandler>
  make(const DeviceDocumentPlugin& plug, const score::DocumentContext& ctx)
      = 0;
};
}