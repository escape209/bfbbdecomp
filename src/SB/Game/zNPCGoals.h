#ifndef ZNPCGOALS_H
#define ZNPCGOALS_H

// Not present in dwarf, so may be inaccurate
// Inspired by en_NPCTYPES
enum en_NPCGOALS
{
    NPC_GOAL_JELLYATTACK = 'NGJ0',
    NPC_GOAL_JELLYBUMPED,
    NPC_GOAL_JELLYBIRTH,
    NPC_GOAL_EXIST = 'NGB0',
    NPC_GOAL_BOSSSANDYIDLE,
    NPC_GOAL_BOSSSANDYTAUNT,
    NPC_GOAL_BOSSSANDYCHASE,
    NPC_GOAL_BOSSSANDYMELEE,
    NPC_GOAL_BOSSSANDYNOHEAD,
    NPC_GOAL_BOSSSANDYELBOWDROP,
    NPC_GOAL_BOSSSANDYLEAP,
    NPC_GOAL_BOSSSANDYSIT,
    NPC_GOAL_BOSSSANDYGETUP,
    NPC_GOAL_BOSSSANDYRUNTOROPE,
    NPC_GOAL_BOSSSANDYCLOTHESLINE,
    NPC_GOAL_BOSSSB1IDLE,
    NPC_GOAL_BOSSSB1TAUNT,
    NPC_GOAL_BOSSSB1STOMP,
    NPC_GOAL_BOSSSB1SMASH,
    NPC_GOAL_BOSSSB1DEFLATE,
    NPC_GOAL_BOSSSB2INTRO,
    NPC_GOAL_BOSSSB2IDLE,
    NPC_GOAL_BOSSSB2TAUNT,
    NPC_GOAL_BOSSSB2DIZZY,
    NPC_GOAL_BOSSSB2HIT,
    NPC_GOAL_BOSSSB2HUNT,
    NPC_GOAL_BOSSSB2SWIPE,
    NPC_GOAL_BOSSSB2CHOP,
    NPC_GOAL_BOSSSB2KARATE,
    NPC_GOAL_BOSSSB2DEATH,
    NPC_GOAL_BPLANKTONIDLE,
    NPC_GOAL_BPLANKTONATTACK,
    NPC_GOAL_BPLANKTONAMBUSH,
    NPC_GOAL_BPLANKTONFLANK,
    NPC_GOAL_BPLANKTONEVADE,
    NPC_GOAL_BPLANKTONHUNT,
    NPC_GOAL_BPLANKTONTAUNT,
    NPC_GOAL_BPLANKTONMOVE,
    NPC_GOAL_BPLANKTONSTUN,
    NPC_GOAL_BPLANKTONFALL,
    NPC_GOAL_BPLANKTONDIZZY,
    NPC_GOAL_BPLANKTONBEAM,
    NPC_GOAL_BPLANKTONWALL,
    NPC_GOAL_BPLANKTONMISSLE,
    NPC_GOAL_BPLANKTONBOMB,
    NPC_GOAL_BOSSPATIDLE,
    NPC_GOAL_BOSSPATTAUNT,
    NPC_GOAL_BOSSPATHIT,
    NPC_GOAL_BOSSPATSPIT,
    NPC_GOAL_BOSSPATRUN,
    NPC_GOAL_BOSSPATSMACK,
    NPC_GOAL_BOSSPATFREEZE,
    NPC_GOAL_BOSSPATSPAWN,
    NPC_GOAL_BOSSPATSPIN,
    NPC_GOAL_BOSSPATFUDGE,
    NPC_GOAL_DUPLOLIVE = 'NGD0',
    NPC_GOAL_DUPLODEAD,
    NPC_GOAL_NOTICE = 'NGR0',
    NPC_GOAL_TAUNT,
    NPC_GOAL_EVADE,
    NPC_GOAL_GOHOME,
    NPC_GOAL_ALERT,
    NPC_GOAL_ALERTFODDER,
    NPC_GOAL_ALERTFODBOMB,
    NPC_GOAL_ALERTFODBZZT,
    NPC_GOAL_ALERTCHOMPER,
    NPC_GOAL_ALERTHAMMER,
    NPC_GOAL_ALERTTARTAR,
    NPC_GOAL_ALERTGLOVE,
    NPC_GOAL_ALERTMONSOON,
    NPC_GOAL_ALERTSLEEPY,
    NPC_GOAL_ALERTARF,
    NPC_GOAL_ALERTPUPPY,
    NPC_GOAL_ALERTCHUCK,
    NPC_GOAL_ALERTTUBELET,
    NPC_GOAL_ALERTSLICK,
    NPC_GOAL_CHASE,
    NPC_GOAL_ATTACKCQC,
    NPC_GOAL_ATTACKFODDER,
    NPC_GOAL_ATTACKCHOMPER,
    NPC_GOAL_ATTACKHAMMER,
    NPC_GOAL_ATTACKTARTAR,
    NPC_GOAL_ATTACKMONSOON,
    NPC_GOAL_ATTACKARF,
    NPC_GOAL_ATTACKARFMELEE,
    NPC_GOAL_ATTACKCHUCK,
    NPC_GOAL_ATTACKSLICK,
    NPC_GOAL_DEFLATE,
    NPC_GOAL_TUBEPAL,
    NPC_GOAL_TUBEDUCKLING,
    NPC_GOAL_TUBEATTACK,
    NPC_GOAL_TUBELASSO,
    NPC_GOAL_TUBEBIRTH,
    NPC_GOAL_TUBEBONKED,
    NPC_GOAL_TUBEDYING,
    NPC_GOAL_TUBEDEAD,
    NPC_GOAL_DOGLAUNCH,
    NPC_GOAL_DOGBARK,
    NPC_GOAL_DOGDASH,
    NPC_GOAL_DOGPOUNCE,
    NPC_GOAL_TELEPORT,
    NPC_GOAL_HOKEYPOKEY,
    NPC_GOAL_LASSOBASE,
    NPC_GOAL_LASSOGRAB,
    NPC_GOAL_LASSOTHROW,
    NPC_GOAL_EVILPAT,
    NPC_GOAL_STUNNED,
    NPC_GOAL_PATCARRY,
    NPC_GOAL_PATTHROW,
    NPC_GOAL_DAMAGE,
    NPC_GOAL_KNOCK,
    NPC_GOAL_R54,
    NPC_GOAL_BASHED,
    NPC_GOAL_R56,
    NPC_GOAL_WOUND,
    NPC_GOAL_AFTERLIFE,
    NPC_GOAL_RESPAWN,
    NPC_GOAL_SCRIPT = 'NGS0',
    NPC_GOAL_SCRIPTANIM,
    NPC_GOAL_SCRIPTATTACK,
    NPC_GOAL_SCRIPTMOVE,
    NPC_GOAL_SCRIPTTALK,
    NPC_GOAL_SCRIPTFOLLOW,
    NPC_GOAL_SCRIPTLEAD,
    NPC_GOAL_SCRIPTWAIT,
    NPC_GOAL_CHEER,
    NPC_GOAL_HURT,
    NPC_GOAL_PLAYERNEAR,
    NPC_GOAL_TALK,
    NPC_GOAL_CHATTER,
    NPC_GOAL_SPEAK,
    NPC_GOAL_BALLOON,
    NPC_GOAL_BOYRIDE,
    NPC_GOAL_BOYFALL,
    NPC_GOAL_BOYWEEP,
    NPC_GOAL_BOYSWIM,
    NPC_GOAL_IDLE = 'NGN0',
    NPC_GOAL_WANDER,
    NPC_GOAL_PATROL,
    NPC_GOAL_FIDGET,
    NPC_GOAL_WAITING,
    NPC_GOAL_DEAD,
    NPC_GOAL_NOMANLAND,
    NPC_GOAL_LIMBO,
    NPC_GOAL_DEVANIMCYCLE = 'NGX0',
    NPC_GOAL_DEVANIMSPIN,
    NPC_GOAL_DEVANIMHERO,
    NPC_GOAL_HERE = 'NGM0',
    NPC_GOAL_KJIDLE,
    NPC_GOAL_KJBORED,
    NPC_GOAL_KJSPAWNKIDS,
    NPC_GOAL_KJTAUNT,
    NPC_GOAL_KJSHOCKGROUND,
    NPC_GOAL_KJDAMAGE,
    NPC_GOAL_KJDEATH,
    NPC_GOAL_PRAWNIDLE,
    NPC_GOAL_PRAWNBEAM,
    NPC_GOAL_PRAWNBOWL,
    NPC_GOAL_PRAWNDAMAGE,
    NPC_GOAL_PRAWNDEATH,
    NPC_GOAL_DUTCHMANNIL,
    NPC_GOAL_DUTCHMANINITIATE,
    NPC_GOAL_DUTCHMANIDLE,
    NPC_GOAL_DUTCHMANDISAPPEAR,
    NPC_GOAL_DUTCHMANTELEPORT,
    NPC_GOAL_DUTCHMANREAPPEAR,
    NPC_GOAL_DUTCHMANBEAM,
    NPC_GOAL_DUTCHMANFLAME,
    NPC_GOAL_DUTCHMANPOSTFLAME,
    NPC_GOAL_DUTCHMANCAUGHT,
    NPC_GOAL_DUTCHMANDAMAGE,
    NPC_GOAL_DUTCHMANDEATH,
    NPC_GOAL_TIKIIDLE = 'NGT0',
    NPC_GOAL_TIKIPATROL,
    NPC_GOAL_TIKIHIDE,
    NPC_GOAL_TIKICOUNT,
    NPC_GOAL_TIKIDYING,
    NPC_GOAL_TIKIDEAD
};

struct xFactory;

void zNPCGoals_RegisterTypes(xFactory* fac);

#endif