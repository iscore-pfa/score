// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "FullViewIntervalHeader.hpp"
#include "FullViewIntervalView.hpp"
#include "FullViewIntervalPresenter.hpp"

#include <Process/Style/ScenarioStyle.hpp>
#include <Scenario/Document/Interval/IntervalHeader.hpp>
#include <Scenario/Document/Interval/IntervalModel.hpp>

#include <score/graphics/GraphicsItem.hpp>
#include <score/widgets/WidgetWrapper.hpp>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QList>
#include <QPoint>

#include <cmath>

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;


namespace Scenario
{
class TimeSignatureItem
    : public QObject
    , public QGraphicsItem
{
  double m_width{100.};
  const IntervalPresenter& m_itv;
public:
  TimeSignatureItem(const IntervalPresenter& itv, QGraphicsItem* parent)
    : QGraphicsItem{parent}
    , m_itv{itv}
  {


  }

  void setWidth(double w)
  {
    prepareGeometryChange();
    m_width = w;
  }

  QRectF boundingRect() const final override
  {
    return {0., 0., m_width, 10.};
  }

  void paint(
      QPainter* painter,
      const QStyleOptionGraphicsItem* option,
      QWidget* widget) override
  {
    painter->setPen(Qt::white);

    enum MaxZoomLevel {
      None = 0,
      Whole = 1,
      Half = 2,
      Quarter = 4,
      Eighth = 8,
      Sixteenth = 16,
      Thirtysecond = 32,
      Sixteenfourth = 64
    };
    auto zoomLevel = Whole;
    double spacing = m_itv.zoomRatio();

    for(auto& [time, sig] : m_itv.model().timeSignatureMap())
    {

    }
  }
};

class TimeSignatureHandle
    : public QObject
    , public QGraphicsItem
{
public:
  TimeSignatureHandle(const IntervalModel& itv, QGraphicsItem* parent)
    : QGraphicsItem{parent}
  {

  }

  QRectF boundingRect() const final override
  {
    return {0., 0., 10., 15.};
  }

  void paint(
      QPainter* painter,
      const QStyleOptionGraphicsItem* option,
      QWidget* widget) override
  {
    painter->fillRect(boundingRect(), Qt::gray);
  }

  void mousePressEvent(QGraphicsSceneMouseEvent* mv) override
  {
    mv->accept();

  }
  void mouseMoveEvent(QGraphicsSceneMouseEvent* mv) override
  {
    mv->accept();

  }
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* mv) override
  {
    mv->accept();

  }

};

FullViewIntervalHeader::FullViewIntervalHeader(
    const IntervalPresenter& itv,
    const score::DocumentContext& ctx,
    QGraphicsItem* parent)
    : IntervalHeader{parent}, m_bar{ctx, this}
{
  this->setCacheMode(QGraphicsItem::NoCache);
  this->setFlag(QGraphicsItem::ItemClipsChildrenToShape, false);
  m_bar.setPos(10., 4.);

  con(m_bar, &AddressBarItem::needRedraw, this, [&]() { update(); });
  auto bar = new TimeSignatureItem{itv, this};
}

AddressBarItem& FullViewIntervalHeader::bar()
{
  return m_bar;
}

void FullViewIntervalHeader::setWidth(double width)
{
  prepareGeometryChange();
  m_width = width;
  if (this->cursor().shape() != openCursor().shape())
    this->setCursor(openCursor());
  update();
}

QRectF FullViewIntervalHeader::boundingRect() const
{
  return {0., 0., m_width, headerHeight()};
}

void FullViewIntervalHeader::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
  painter->setRenderHint(QPainter::Antialiasing, false);

  const auto& skin = Process::Style::instance();
  painter->setPen(skin.IntervalHeaderSeparator);
  painter->drawLine(
      QPointF{0., (double)headerHeight()},
      QPointF{m_width, (double)headerHeight()});

  double textWidth = m_bar.width();

  // If the centered text is hidden, we put it at the left so that it's on the
  // view.
  // We have to compute the visible part of the header
  auto view = getView(*this);
  if (!view)
    return;

  // Note: if the interval always has its pos() in (0; 0), we can
  // safely remove the call to mapToScene.

  const double text_left
      = view->mapFromScene(
                mapToScene(QPointF{10., 0.}))
            .x();
  const double text_right = text_left + textWidth;
  double x = 10.;
  const constexpr double min_x = 10.;
  const double max_x = view->width() - 30.;

  if (text_left <= min_x)
  {
    // Compute the pixels needed to add to have top-left at 0
    x = x - text_left + min_x;
  }
  else if (text_right >= max_x)
  {
    // Compute the pixels needed to add to have top-right at max
    x = x - text_right + max_x;
  }
  x = std::max(x, 5.);

  if (std::abs(m_bar.pos().x() - x) > 0.1)
    m_bar.setPos(x, 4.);

}
}
