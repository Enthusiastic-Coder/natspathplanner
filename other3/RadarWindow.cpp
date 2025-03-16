
#include "RadarWindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QInputDialog>
#include <QTimer>
#include <cmath>

RadarWindow::RadarWindow(QWidget *parent) : QWidget(parent) {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&RadarWindow::update));
    timer->start(1000); // Refresh radar every second
}

void RadarWindow::addAircraft(Aircraft aircraft) {
    aircraftList.push_back(aircraft);
    update();
}

void RadarWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw radar background
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width(), height());

    // Draw aircraft positions and labels
    for (const auto &aircraft : aircraftList) {
        int x = static_cast<int>(aircraft.longitude * 50) % width();
        int y = static_cast<int>(aircraft.latitude * 50) % height();

        // Draw aircraft blip
        painter.setBrush(Qt::green);
        painter.drawEllipse(QPoint(x, y), 5, 5);
        painter.drawText(QPoint(x + 10, y), QString::fromStdString(aircraft.callsign));

        // Highlight selected aircraft
        if (selectedAircraft == &aircraft) {
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPoint(x, y), 7, 7);
        }
    }
}

void RadarWindow::mousePressEvent(QMouseEvent *event) {
    int clickX = event->pos().x();
    int clickY = event->pos().y();

    for (auto &aircraft : aircraftList) {
        int x = static_cast<int>(aircraft.longitude * 50) % width();
        int y = static_cast<int>(aircraft.latitude * 50) % height();

        // Check if clicked position is near aircraft
        if (std::abs(clickX - x) < 10 && std::abs(clickY - y) < 10) {
            selectedAircraft = &aircraft;
            openVectoringDialog();
            break;
        }
    }
}

void RadarWindow::openVectoringDialog() {
    if (!selectedAircraft) return;

    bool ok;
    double newHeading = QInputDialog::getDouble(this, "Assign Heading", "Enter new heading:", selectedAircraft->heading, 0, 360, 1, &ok);
    if (ok) selectedAircraft->assignHeading(newHeading);

    double newSpeed = QInputDialog::getDouble(this, "Assign Speed", "Enter new speed (knots):", selectedAircraft->speed, 100, 300, 1, &ok);
    if (ok) selectedAircraft->assignSpeed(newSpeed);

    double newAltitude = QInputDialog::getDouble(this, "Assign Altitude", "Enter new altitude (feet):", selectedAircraft->altitude, 1000, 10000, 100, &ok);
    if (ok) selectedAircraft->assignAltitude(newAltitude);

    update(); // Refresh radar
}
