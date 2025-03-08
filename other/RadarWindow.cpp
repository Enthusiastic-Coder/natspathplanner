
#include "RadarWindow.h"
#include <QPainter>

void RadarWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw radar background
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width(), height());

    // Draw aircraft positions
    for (const auto &aircraft : aircraftList) {
        int x = mapLongitudeToPixel(aircraft.longitude);
        int y = mapLatitudeToPixel(aircraft.latitude);
        painter.setBrush(Qt::green);
        painter.drawEllipse(QPoint(x, y), 5, 5);
        painter.drawText(QPoint(x + 10, y), QString::fromStdString(aircraft.callsign));
    }
}
