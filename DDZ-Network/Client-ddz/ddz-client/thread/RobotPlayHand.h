#pragma once

#include <QRunnable>
#include "Player.h"

class RobotPlayHand : public QObject, public QRunnable
{
	Q_OBJECT
public:
	explicit RobotPlayHand(Player* player, QObject* parent = nullptr);

protected:
	void run() override;

private:
	Player* m_player;
	
};

