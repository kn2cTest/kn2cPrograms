#include "playkickoffour.h"

PlayKickoffOur::PlayKickoffOur(WorldModel *worldmodel, QObject *parent) :
    Play("PlayKickoffOur", worldmodel, parent)
{
    tGolie=new TacticGoalie(wm);

    tDefenderLeft=new TacticDefender(wm);
    tDefenderRight=new TacticDefender(wm);

    tAttackerLeft = new TacticAttacker(wm);
    tAttackerMid = new TacticAttacker(wm);
    tAttackerRight = new TacticAttacker(wm);
}

int PlayKickoffOur::enterCondition()
{
    if(wm->cmgs.ourKickoff())
        return 100;
    else
        return 0;
}

void PlayKickoffOur::initRole()
{
    QList<int> activeAgents=wm->kn->ActiveAgents();
    numberOfPlayers = activeAgents.size();
    activeAgents.removeOne(wm->ref_goalie_our);
    wm->ourRobot[wm->ref_goalie_our].Role = AgentRole::Golie;
    switch (activeAgents.length()) {
    case 1:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        break;
    case 2:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
        break;
    case 3:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        break;
    case 4:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerLeft;
        break;
    case 5:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerLeft;
        break;
    }
}

void PlayKickoffOur::setTactics(int index)
{
    switch (wm->ourRobot[index].Role) {
    case AgentRole::Golie:
        tactics[index] = tGolie;
        break;
    case AgentRole::DefenderLeft:
        tactics[index] = tDefenderLeft;
        break;
    case AgentRole::DefenderRight:
        tactics[index] = tDefenderRight;
        break;
    case AgentRole::AttackerMid:
        tactics[index] = tAttackerMid;
        break;
    case AgentRole::AttackerRight:
        tactics[index] = tAttackerRight;
        break;
    case AgentRole::AttackerLeft:
        tactics[index] = tAttackerLeft;
        break;
    default:
        break;
    }
}

void PlayKickoffOur::setPositions()
{
    Position goaliePos,leftDefPos,rightDefPos;
    zonePositions(tDefenderLeft->getID(),tDefenderRight->getID(),-1,goaliePos,leftDefPos,rightDefPos);
    tDefenderLeft->setIdlePosition(leftDefPos);
    tDefenderRight->setIdlePosition(rightDefPos);
    tGolie->setIdlePosition(goaliePos);

    tAttackerMid->setIdlePosition(wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x-100,wm->ball.pos.loc.y),Field::oppGoalCenter));
    tAttackerRight->setIdlePosition(wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x - 250 , 1550),Field::oppGoalCenter));
    tAttackerLeft->setIdlePosition(wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x - 250 , -1550),Field::oppGoalCenter));
}

void PlayKickoffOur::execute()
{
    QList<int> activeAgents=wm->kn->ActiveAgents();

    initRole();

    for(int i=0;i<activeAgents.size();i++)
        setTactics(activeAgents.at(i));

    setPositions();

    if( wm->cmgs.canKickBall() )
        wm->ourRobot[tAttackerMid->getID()].Status =  AgentStatus::Kicking;

}
