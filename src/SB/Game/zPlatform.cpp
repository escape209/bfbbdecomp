#include "zPlatform.h"
#include "zEnt.h"
#include "zEntPlayer.h"
#include "xEntDrive.h"
#include "zParEmitter.h"

#include "xMath.h"
#include "xMath3.h"
#include "xstransvc.h"
#include "zGlobals.h"

#include <types.h>

zParEmitter* sEmitTremble;
zParEmitter* sEmitBreakaway;

// Taken from zPlatform.s
// Defining these here makes the stringBase0 offsets match in the later functions.
char* str1 = "";
char* str2 = "Idle";
char* str3 = "Spring";
char* str4 = "teeter_totter_pat";
char* str5 = "teeter_totter_pat_bind";
char* str6 = "PAREMIT_PLAT_TREMBLE";
char* str7 = "PAREMIT_PLAT_BREAKAWAY";
char* str8 = "skatepark_bumper";
char* str9 = "skatepark_flipper";
char* str10 = "Check1";

static void zPlatformTranslate(xEnt* xent, xVec3* dpos, xMat4x3* dmat);
void zPlatform_Move(xEnt* entPlat, xScene* s, F32 dt, xEntFrame* frame);
static void zPlatform_Tremble(zPlatform* plat, F32 ampl, F32 freq, F32 dur);

static void genericPlatRender(xEnt* ent)
{
    if (!ent->model || !xEntIsVisible(ent))
    {
        return;
    }

    xModelRender(ent->model);
}

void zPlatform_Init(void* plat, void* asset)
{
    zPlatform_Init((zPlatform*)plat, (xEntAsset*)asset);
}

void zPlatform_Init(zPlatform* plat, xEntAsset* asset)
{
    xAnimFile* spring_file;
    xAnimFile* idle_file;
    xPlatformAsset* passet = (xPlatformAsset*)(&asset[1]);
    xEntMotionAsset* emasset = (xEntMotionAsset*)(&passet[1]);

    // asset pointer points to packed structure with more than just an xEntAsset inside!
    zEntInit(plat, asset, 'PLAT');

    plat->passet = passet;
    plat->subType = passet->type;

    if (plat->linkCount != 0)
    {
        plat->link = (xLinkAsset*)(&emasset[1]);
    }
    else
    {
        plat->link = NULL;
    }

    plat->update = zPlatform_Update;
    plat->move = zPlatform_Move;
    plat->eventFunc = zPlatformEventCB;
    plat->transl = zPlatformTranslate;
    plat->render = genericPlatRender;

    plat->am = NULL;
    plat->bm = NULL;
    plat->state = ZPLATFORM_STATE_INIT;
    plat->plat_flags = 0;
    plat->fmrt = NULL;
    plat->pauseMult = 1.0f;
    plat->pauseDelta = 0.0f;

    if (plat->subType == ZPLATFORM_SUBTYPE_BREAKAWAY)
    {
        plat->collis = (xEntCollis*)xMemAlloc(gActiveHeap, sizeof(xEntCollis), 0);

        xModelInstance* modelInst = NULL;

        plat->collis->chk = 0;
        plat->collis->pen = 0;

        plat->collis->colls[18].flags = 0;

        plat->am = plat->model;

        U32 size = 0;

        if (passet->ba.bustModelID != 0)
        {
            modelInst = (xModelInstance*)xSTFindAsset(passet->ba.bustModelID, &size);
        }

        if (modelInst != NULL)
        {
            xEntLoadModel(plat, (RpAtomic*)modelInst);
            plat->bm = plat->model;
        }
        else
        {
            plat->bm = NULL;
        }

        plat->model = plat->am;
        plat->collModel = NULL;
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_SPRINGBOARD)
    {
        xAnimFile* animFile = NULL;

        if (passet->sb.animID[0] != NULL)
        {
            spring_file = (xAnimFile*)xSTFindAsset(passet->sb.animID[0], NULL);
        }
        else
        {
            spring_file = NULL;
        }

        if (passet->sb.animID[1] != NULL)
        {
            idle_file = (xAnimFile*)xSTFindAsset(passet->sb.animID[1], NULL);
        }
        else
        {
            idle_file = NULL;
        }

        if (spring_file != NULL || idle_file != NULL)
        {
            gxAnimUseGrowAlloc = TRUE;

            plat->atbl = xAnimTableNew("", NULL, 0x0);
            xAnimTableNewState(plat->atbl, "Idle", 0x10, 0x0, 1.0f, NULL, NULL, 0.0f, NULL, NULL,
                               xAnimDefaultBeforeEnter, NULL, NULL);

            if (spring_file != NULL)
            {
                xAnimTableNewState(plat->atbl, "Spring", 0x20, 0, 1.0f, NULL, NULL, 0.0f,
                                   NULL, NULL, xAnimDefaultBeforeEnter, NULL, NULL);
                xAnimTableNewTransition(plat->atbl, "Spring", "Idle", NULL, NULL,
                                        0x10, 0, 0.0f, 0.0f, 0, 0, 0.1f, NULL);

                animFile = xAnimFileNew(spring_file, "", 0, NULL);
                xAnimTableAddFile(plat->atbl, animFile, "Spring");
            }

            if (idle_file != NULL)
            {
                animFile = xAnimFileNew(idle_file, "", 0, NULL);
                xAnimTableAddFile(plat->atbl, animFile, "Idle");
            }
            else
            {
                xAnimTableAddFile(plat->atbl, animFile, "Idle");
                plat->atbl->StateList->Speed = 0.0f;
            }

            // TODO: if this isn't matching, it's because the globals struct isn't defined correctly
            //       Figure out why that is
            gxAnimUseGrowAlloc = FALSE;
            xAnimPoolAlloc(&globals.scenePreload->mempool, plat, plat->atbl, plat->model);
        }
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_FM)
    {
        plat->fmrt = (zPlatFMRunTime*)xMemAlloc(gActiveHeap, sizeof(zPlatFMRunTime), 0);
    }

    xEntMotionInit(&plat->motion, plat, emasset);
    xEntDriveInit(&plat->drv, plat);

    plat->drv.flags = 1;

    if (plat->asset->modelInfoID == xStrHash("teeter_totter_pat") ||
        plat->asset->modelInfoID == xStrHash("teeter_totter_pat_bind"))
    {
        plat->plat_flags |= 2;
    }
}

void zPlatform_Setup(zPlatform* ent, xScene* sc)
{
    zEntSetup((zEnt*)ent);
    sEmitTremble = zParEmitterFind("PAREMIT_PLAT_TREMBLE");
    sEmitBreakaway = zParEmitterFind("PAREMIT_PLAT_BREAKAWAY");
    if (ent->subType == ZPLATFORM_SUBTYPE_PADDLE)
    {
        ent->tmr = 1e-9;
        ent->state = 2;
        ent->ctr = ent->passet->paddle.startOrient;
    }
}

void zPlatform_Save(zPlatform* ent, xSerial* s)
{
    zEntSave(ent, s);
}

void zPlatform_Load(zPlatform* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

void zPlatform_Reset(zPlatform* plat, xScene* sc)
{
    if (plat->subType == ZPLATFORM_SUBTYPE_BREAKAWAY)
    {
        plat->model = plat->am;
        plat->collModel = NULL;
    }

    zEntReset(plat);

    xEntMotionInit(&plat->motion, plat, (xEntMotionAsset*)(&plat->passet[1]));
    xEntMotionReset(&plat->motion, sc);

    plat->plat_flags = 0x1;
    if (plat->subType == ZPLATFORM_SUBTYPE_PLATFORM)
    {
        plat->state = ZPLATFORM_STATE_UNK1;
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_BREAKAWAY)
    {
        plat->tmr = plat->passet->ba.ba_delay;
        plat->state = ZPLATFORM_STATE_INIT;
        plat->pflags &= 0xF9;
        plat->collis->chk = 0x0;

        xVec3Copy(&plat->frame->vel, (const xVec3*)&g_O3);

        plat->bound.mat = (xMat4x3*)plat->model->Mat;
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_MECH)
    {
        plat->state = (U16)plat->motion.mech.state;
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_SPRINGBOARD)
    {
        plat->tmr = -1.0f;
        plat->ctr = 0;
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_PADDLE)
    {
        plat->tmr = 1e-9f;
        plat->state = ZPLATFORM_STATE_UNK3;
        plat->ctr = plat->passet->paddle.startOrient;
    }
    else if (plat->subType == ZPLATFORM_SUBTYPE_FM)
    {
        for (U32 i = 0; i < 12; i++)
        {
            plat->fmrt->flags = 0;

            plat->fmrt->tmrs[i] = 0.0f;
            plat->fmrt->ttms[i] = 0.0f;
            plat->fmrt->atms[i] = 0.0f;
            plat->fmrt->dtms[i] = 0.0f;
            plat->fmrt->vms[i] = 0.0f;
            plat->fmrt->dss[i] = 0.0f;
        }
    }

    if (plat->motion.type == 0x3)
    {
        plat->src = plat->motion.mp.src;
    }

    plat->chkby &= 0xE3;

    if (plat->passet->flags & 0x4)
    {
        plat->chkby |= 0x18;
    }

    plat->bupdate(plat, (xVec3*)&plat->model->Mat->pos);

    plat->moving = FALSE;

    if (plat->asset->modelInfoID == xStrHash("teeter_totter_pat") ||
        plat->asset->modelInfoID == xStrHash("teeter_totter_pat_bind"))
    {
        plat->plat_flags |= 0x2;
    }

    plat->pauseMult = 1.0f;
    plat->pauseDelta = 0.0f;
}

static U32 zMechIsStartingForth(zPlatform* ent, U16 param_2)
{
    if (ent->motion.asset->mech.type == 4)
    {
        return param_2 == 1;
    }
    else
    {
        return param_2 == 0;
    }
}

static U32 zMechIsStartingBack(zPlatform* ent, U16 param_2)
{
    if (ent->motion.asset->mech.type == 4)
    {
        return param_2 == 4;
    }
    else
    {
        return param_2 == 3;
    }
}

static F32 SolvePaddleMotion(zPlatform* plat, F32* time, F32 tmr)
{
    return 0.0f;
}

void zPlatform_PaddleStartRotate(xEnt* entplat, S32 direction, S32 stutter)
{
    zPlatform* plat = (zPlatform*)entplat;
    F32 time[3];

    if (stutter != 0)
    {
        if (direction > 0)
        {
            plat->state = ZPLATFORM_STATE_UNK4;
        }
        else if (direction < 0)
        {
            plat->state = ZPLATFORM_STATE_UNK1;
        }
    }
    else
    {
        if (direction > 0)
        {
            plat->state = ZPLATFORM_STATE_UNK3;
            plat->ctr += 1;

            if (plat->ctr >= plat->passet->paddle.countOrient)
            {
                plat->ctr = 0;
            }
        }
        else if (direction < 0)
        {
            plat->state = ZPLATFORM_STATE_UNK2;
            plat->ctr -= 1;

            if (plat->ctr < 0)
            {
                plat->ctr = plat->passet->paddle.countOrient - 1;
            }
        }
    }

    SolvePaddleMotion(plat, time, -1.0f);
    plat->tmr = time[0] + time[1] + time[2];
}

U32 zPlatform_PaddleCollide(xCollis* coll, const xVec3* hitsource, const xVec3* hitvel, U32 worldSpaceNorm)
{
    zPlatform* plat = (zPlatform*)coll->optr;
    xVec3 locnorm;
    xVec3 lochitsrc;
    xVec3 lochitvel;
    xVec3 hitsrcbot;
    xVec3 hitsrctop;
    xRay3 hitsrcray;
    xCollis hitsrccoll;
    S32 direction;
    S32 stutter;

    if (plat->state != 0)
    {
        return 0;
    }
    else if (worldSpaceNorm != 0)
    {
        xMat3x3Tolocal(&locnorm, (xMat3x3 *)plat->model->Mat, &coll->norm);
    }
    else
    {
        locnorm = coll->norm;
    }

    xMat4x3Tolocal(&lochitsrc, (xMat4x3 *)plat->model->Mat, hitsource);
    xMat3x3Tolocal(&lochitvel, (xMat3x3 *)plat->model->Mat, hitvel);

    if
    (
    ((F32)__fabs(locnorm.y) > (F32)__fabs(locnorm.x)) &&
    ((F32)__fabs(locnorm.y) > (F32)__fabs(locnorm.z))
    )
    {
        return 0;
    }
    else
    {
        if (((F32)__fabs(lochitvel.y) > (F32)__fabs(lochitvel.x)) && ((F32)__fabs(lochitvel.y) > (F32)__fabs(lochitvel.z)))
        {
            return 0;
        }
        else
        {
            xVec3Normalize(&locnorm, &locnorm);
            xVec3Normalize(&lochitvel, &lochitvel);

            if (xVec3Dot(&locnorm, &lochitvel) > -0.7071f) // -cos(45)
            {
                return 0;
            }
            else
            {
                hitsrcbot = lochitsrc;
                hitsrctop = lochitsrc;
                hitsrcbot.y -= 5.0f;
                hitsrctop.y += 5.0f;

                xMat4x3Toworld(&hitsrcbot, (xMat4x3 *)plat->model->Mat, &hitsrcbot);
                xMat4x3Toworld(&hitsrctop, (xMat4x3 *)plat->model->Mat, &hitsrctop);

                hitsrcray.origin.x = hitsrcbot.x;
                hitsrcray.origin.y = hitsrcbot.y;
                hitsrcray.origin.z = hitsrcbot.z;
                hitsrcray.dir.x = hitsrctop.x - hitsrcbot.x;
                hitsrcray.dir.y = hitsrctop.y - hitsrcbot.y;
                hitsrcray.dir.z = hitsrctop.z - hitsrcbot.z;
                hitsrcray.min_t = 0.0f;
                hitsrcray.max_t = xVec3Normalize(&hitsrcray.dir, &hitsrcray.dir);
                hitsrcray.flags = 0xc00;

                xRayHitsBound(&hitsrcray, &plat->bound, &hitsrccoll);

                if (!(hitsrccoll.flags & 1))
                {
                    return 0;
                }
                else
                {
                    if ((F32)__fabs(lochitvel.x) < (F32)__fabs(lochitvel.z))
                    {
                        if (!((lochitsrc.x > 0.0f) ^ (lochitsrc.z > 0.0f)))
                        {
                            direction = 1;
                        }
                        else
                        {
                            direction = -1;
                        }
                    }
                    else if (!((lochitsrc.x > 0.0f) ^ (lochitsrc.z > 0.0f)))
                    {
                        direction = -1;
                    }
                    else
                    {
                        direction = 1;
                    }

                    stutter = 0;

                    if (((direction == 1) && (!(plat->passet->paddle.paddleFlags & 1) || !(plat->passet->paddle.paddleFlags & 4) && (plat->ctr + 1 == plat->passet->paddle.countOrient))))
                    {
                        stutter = 1;
                    }
                    if ((direction == -1) && (!(plat->passet->paddle.paddleFlags & 2) || !(plat->passet->paddle.paddleFlags & 4) && (plat->ctr == 0)))
                    {
                        stutter = 1;
                    }
                    if (stutter == 0)
                    {
                        if (direction > 0)
                        {
                            zEntEvent(plat, eEventHit_PaddleLeft);
                        }
                        else
                        {
                            zEntEvent(plat, eEventHit_PaddleRight);
                        }
                    }

                    zPlatform_PaddleStartRotate(plat, direction, stutter);

                    return 1;
                }
            }
        }
    }
}

void zPlatform_Update(xEnt* entplat, xScene* sc, F32 dt)
{

}

void zPlatform_Move(xEnt* entPlat, xScene* s, F32 dt, xEntFrame* frame)
{
    zPlatform* plat = (zPlatform*)entPlat;
    xEntMotionMove(&plat->motion, s, dt, frame);
    xEntDriveUpdate(&plat->drv, s, dt, NULL);
}

void zPlatform_Shake(zPlatform* plat, F32 _unused, F32 ampl, F32 freq)
{
    xFFXShakeState* ss;
    xFFX* sfkt;

    ss = xFFXShakeAlloc();
    if (ss != NULL)
    {
        sfkt = xFFXAlloc();
        if (sfkt == NULL)
        {
            xFFXShakeFree(ss);
            ss = NULL;
        }
        else
        {
            ss->disp.x = 0.0f;
            ss->disp.y = -ampl;
            ss->disp.z = 0.0f;
            ss->dur = 1.0f;
            ss->alpha = -7.0f / ss->dur;
            ss->freq = freq;
            ss->tmr = 0.0f;
            sfkt->doEffect = xFFXShakeUpdateEnt;
            sfkt->fdata = ss;
            xFFXAddEffect(plat, sfkt);
        }
    }
}

static void zPlatform_Tremble(zPlatform* plat, F32 ampl, F32 freq, F32 dur)
{
    xFFXShakeState* ss;
    xFFX* sfkt;
    xParEmitterCustomSettings info;
    S32 i;

    ss = xFFXShakeAlloc();
    if (ss != NULL)
    {
        sfkt = xFFXAlloc();
        if (sfkt == NULL)
        {
            xFFXShakeFree(ss);
            return;
        }

        xVec3SMul(&ss->disp, (xVec3*)&globals.camera.mat, ampl);
        ss->dur = dur;
        ss->alpha = 1.0f / dur;
        ss->freq = freq;
        ss->tmr = 0.0f;
        sfkt->doEffect = xFFXShakeUpdateEnt;
        sfkt->fdata = ss;
        xFFXAddEffect(plat, sfkt);
    }

    if (sEmitTremble != NULL)
    {
        info.custom_flags = 0x100;
        info.pos = *xEntGetCenter(plat);
        for (i = 0; i < 25; i++)
        {
            // Emit a particle at every 30 frames?
            xParEmitterEmitCustom(sEmitTremble, 1.0f / 30, &info);
        }
    }
}

static void zPlatform_BreakawayFallFX(zPlatform* ent, F32 dt)
{
    if (sEmitBreakaway != NULL)
    {
        xParEmitterCustomSettings info;
        info.custom_flags = 0x100;
        info.pos = *xEntGetCenter(ent);
        info.pos.y += 0.5f;
        for (int iVar2 = 0; iVar2 < 25; iVar2++)
        {
            xParEmitterEmitCustom(sEmitBreakaway, 1.0f / 30.0f, &info);
        }
    }
}

void zPlatform_Tremble(zPlatform* plat, F32 ampl, F32 freq, F32 dur);

void zPlatform_Mount(zPlatform* ent)
{
    if (ent->subType == ZPLATFORM_SUBTYPE_BREAKAWAY)
    {
        if (ent->state == 0)
        {
            if ((ent->passet->ba.breakflags & 1) && zEntPlayer_IsSneaking())
            {
                ent->state = 1;
            }
            else
            {
                ent->state = 2;

                // Needs to be used or the comparison's operands will be swapped.
                F32 restingSpeed = 0.0f;
                if (ent->passet->fr.fspeed != restingSpeed)
                {
                    zPlatform_Tremble(ent, 0.06f, (2 * PI) * 4, ent->passet->fr.fspeed + 1.0f);
                }
            }
        }
    }
}

void zPlatform_Dismount(zPlatform* ent)
{
    if ((ent->subType == 9) && (ent->state == 1))
    {
        ent->state = 0;
    }
}

static void zPlatformTranslate(xEnt* xent, xVec3* dpos, xMat4x3* dmat)
{
    zPlatform* plat = (zPlatform*)xent;
    xEntDefaultTranslate(xent, dpos, dmat);
    xEntMotionTranslate(&plat->motion, dpos, dmat);
}

static void zPlatFM_EventSetup(zPlatform* plat, const F32* toParam, S32 idx)
{
    F32 ds, atm, ttm, dtm;

    zPlatFMRunTime* fmrt = plat->fmrt;
    if (fmrt->flags & (1 << idx))
    {
        return;
    }

    ds = toParam[0];
    atm = toParam[1];
    ttm = toParam[2];
    dtm = toParam[3];

    if (atm <= 0.0f)
    {
        return;
    }

    if (ttm > atm)
    {
        ttm = atm;
    }
    else if (ttm < 0.0f)
    {
        ttm = 0.0f;
    }

    ttm = atm - ttm;
    if (dtm > ttm)
    {
        dtm = ttm;
    }
    else if (dtm < 0.0f)
    {
        dtm = 0.0f;
    }

    fmrt->tmrs[idx] = atm;
    fmrt->ttms[idx] = atm;
    fmrt->atms[idx] = ttm;
    fmrt->dtms[idx] = dtm;
    fmrt->vms[idx] = 2.0f * ds / (atm - dtm + ttm);
    fmrt->dss[idx] = ds;
    fmrt->flags |= (1 << idx);
}

S32 zPlatformEventCB(xBase* from, xBase* to, U32 toEvent, const F32* toParam, xBase* base3)
{
    return 1;
}
