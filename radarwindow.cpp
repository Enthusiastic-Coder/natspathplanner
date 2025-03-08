
#include "radarwindow.h"
#include <QPainter>
#include <QFont>
#include <cmath>

RadarWindow::RadarWindow(QWidget* parent) : QWidget(parent) {}

void RadarWindow::setAircraftList(const std::vector<Aircraft>& list) {
    aircraftList = list;
    update();
}

void RadarWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    for (const auto& ac : aircraftList) {
        drawAircraft(painter, ac);
    }

    for (size_t i = 0; i < aircraftList.size(); ++i) {
        for (size_t j = i + 1; j < aircraftList.size(); ++j) {
            if (aircraftList[i].isInConflictWith(aircraftList[j], 3.0, 1000.0)) {
                drawConflictAlert(painter, aircraftList[i], aircraftList[j]);
            }
        }
    }
}

void RadarWindow::drawAircraft(QPainter& painter, const Aircraft& aircraft) {
    int x = static_cast<int>((aircraft.longitude + 1) * width() / 2);
    int y = static_cast<int>((1 - (aircraft.latitude - 51) / 0.5) * height());

    painter.setPen(Qt::green);
    painter.drawEllipse(QPoint(x, y), 5, 5);
    painter.drawText(x + 8, y, QString::fromStdString(aircraft.callsign));

    // Example advisory (placeholder for more advanced advisories)
    if (aircraft.altitude_ft < 4000) {
        drawAdvisory(painter, aircraft, "Climb 5000ft");
    }
}

void RadarWindow::drawConflictAlert(QPainter& painter, const Aircraft& ac1, const Aircraft& ac2) {
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    int x1 = static_cast<int>((ac1.longitude + 1) * width() / 2);
    int y1 = static_cast<int>((1 - (ac1.latitude - 51) / 0.5) * height());

    int x2 = static_cast<int>((ac2.longitude + 1) * width() / 2);
    int y2 = static_cast<int>((1 - (ac2.latitude - 51) / 0.5) * height());

    painter.drawLine(x1, y1, x2, y2);
    painter.drawText((x1 + x2) / 2, (y1 + y2) / 2 - 10, "CONFLICT!");
}

void RadarWindow::drawAdvisory(QPainter& painter, const Aircraft& aircraft, const std::string& advisory) {
    int x = static_cast<int>((aircraft.longitude + 1) * width() / 2);
    int y = static_cast<int>((1 - (aircraft.latitude - 51) / 0.5) * height());

    painter.setPen(Qt::yellow);
    painter.drawText(x + 8, y + 15, QString::fromStdString(advisory));
}
