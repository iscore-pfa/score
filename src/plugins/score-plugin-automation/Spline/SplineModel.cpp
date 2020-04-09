// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Process/Dataflow/Port.hpp>

#include <ossia/editor/state/destination_qualifiers.hpp>


#include <Spline/SplineModel.hpp>
#include <Spline/SplinePresenter.hpp>
#include <wobjectimpl.h>
W_OBJECT_IMPL(Spline::ProcessModel)
namespace Spline
{
ProcessModel::ProcessModel(
    const TimeVal& duration,
    const Id<Process::ProcessModel>& id,
    QObject* parent)
    : Process::ProcessModel{duration,
                            id,
                            Metadata<ObjectKey_k, ProcessModel>::get(),
                            parent}
    , outlet{Process::make_value_outlet(Id<Process::Port>(0), this)}
{



  m_spline.points.push_back({0.00, 0.00, 0.0});
  m_spline.points.push_back({0.00, 0.00, 0.0});
  m_spline.points.push_back({0.5, 1.0,  1.0});
  m_spline.points.push_back({0.5, 1.0,  1.0});

  m_spline.points.push_back({1.0, 0.0,  0.0});
  m_spline.points.push_back({1.0, 0.0,  0.0});
  // m_spline.points.push_back({0.50, 0.50, 0.8});

  init();
  metadata().setInstanceName(*this);
}

ProcessModel::~ProcessModel() {}

void ProcessModel::init()
{
  outlet->setCustomData("Out");
  m_outlets.push_back(outlet.get());
  connect(
      outlet.get(),
      &Process::Port::addressChanged,
      this,
      [=](const State::AddressAccessor& arg) {
        addressChanged(arg);
        prettyNameChanged();
        unitChanged(arg.qualifiers.get().unit);
      });
}

QString ProcessModel::prettyName() const noexcept
{
  return address().toString();
}

void ProcessModel::setDurationAndScale(const TimeVal& newDuration) noexcept
{
  // We only need to change the duration.
  setDuration(newDuration);
}

void ProcessModel::setDurationAndGrow(const TimeVal& newDuration) noexcept
{
  setDuration(newDuration);
}

void ProcessModel::setDurationAndShrink(const TimeVal& newDuration) noexcept
{
  setDuration(newDuration);
}

TimeVal ProcessModel::contentDuration() const noexcept
{
  return duration();
}

::State::AddressAccessor ProcessModel::address() const
{
  return outlet->address();
}

void ProcessModel::setAddress(const ::State::AddressAccessor& arg)
{
  outlet->setAddress(arg);
}

State::Unit ProcessModel::unit() const
{
  return outlet->address().qualifiers.get().unit;
}

void ProcessModel::setUnit(const State::Unit& u)
{
  if (u != unit())
  {
    auto addr = outlet->address();
    addr.qualifiers.get().unit = u;
    outlet->setAddress(addr);
    prettyNameChanged();
    unitChanged(u);
  }
}
}

/// Point ///
template <>
void DataStreamReader::read(const ossia::nodes::spline_point& autom)
{
  m_stream << autom.m_x << autom.m_y << autom.m_z;
}

template <>
void DataStreamWriter::write(ossia::nodes::spline_point& autom)
{
  m_stream >> autom.m_x >> autom.m_y >> autom.m_z;
}
template <>
void JSONValueReader::read(const ossia::nodes::spline_point& autom)
{
  val = QJsonArray{autom.x(), autom.y(), autom.z()};
}

template <>
void JSONValueWriter::write(ossia::nodes::spline_point& autom)
{
  auto arr = val.toArray();
  autom.m_x = arr[0].toDouble();
  autom.m_y = arr[1].toDouble();
  autom.m_z = arr[2].toDouble();
}

/// Data ///
template <>
void DataStreamReader::read(const ossia::nodes::spline_data& autom)
{
  m_stream << autom.points;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(ossia::nodes::spline_data& autom)
{
  m_stream >> autom.points;
  checkDelimiter();
}
template <>
void JSONValueReader::read(const ossia::nodes::spline_data& autom)
{
  val = toJsonValueArray(autom.points);
}
template <>
void JSONValueWriter::write(ossia::nodes::spline_data& autom)
{
  autom.points = fromJsonValueArray<std::vector<ossia::nodes::spline_point>>(
      val.toArray());
}

template <>
void DataStreamReader::read(const Spline::ProcessModel& autom)
{
  m_stream << *autom.outlet << autom.m_spline << autom.m_tween;

  insertDelimiter();
}
template <>
void DataStreamWriter::write(Spline::ProcessModel& autom)
{
  autom.outlet = Process::load_value_outlet(*this, &autom);
  m_stream >> autom.m_spline >> autom.m_tween;

  checkDelimiter();
}

template <>
void JSONObjectReader::read(const Spline::ProcessModel& autom)
{
  obj["Outlet"] = toJsonObject(*autom.outlet);
  JSONValueReader v{};
  v.readFrom(autom.m_spline);
  obj["Spline"] = v.val;
  obj["Tween"] = autom.tween();
}

template <>
void JSONObjectWriter::write(Spline::ProcessModel& autom)
{
  JSONObjectWriter writer{obj["Outlet"].toObject()};
  autom.outlet = Process::load_value_outlet(writer, &autom);

  autom.setTween(obj["Tween"].toBool());
  JSONValueWriter v{};
  v.val = obj["Spline"];
  v.writeTo(autom.m_spline);
}
