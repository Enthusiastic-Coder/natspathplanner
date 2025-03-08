
#ifndef RADARWINDOW_H
#define RADARWINDOW_H

#include <QWidget>
#include <QPainter>
#include "aircraft.h"
#include <vector>

class RadarWindow : public QWidget {
    Q_OBJECT

public:
    RadarWindow(QWidget* parent = nullptr);
    void setAircraftList(const std::vector<Aircraft>& aircraftList);
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<Aircraft> aircraftList;
    void drawAircraft(QPainter& painter, const Aircraft& aircraft);
    void drawConflictAlert(QPainter& painter, const Aircraft& ac1, const Aircraft& ac2);
    void drawAdvisory(QPainter& painter, const Aircraft& aircraft, const std::string& advisory);
};

#endif // RADARWINDOW_H
