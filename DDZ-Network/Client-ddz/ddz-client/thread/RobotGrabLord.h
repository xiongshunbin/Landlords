#pragma once

#include <QRunnable>
#include "Player.h"

class RobotGrabLord : public QObject, public QRunnable
{
	Q_OBJECT
public:
	explicit RobotGrabLord(Player* player, QObject* parent = nullptr);

protected:
	void run() override;

private:
	Player* m_player;
};