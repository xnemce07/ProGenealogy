/*!
 *  \file   dbrecordrole.h
 *  \author Leopold Nemcek
 */

#ifndef DBRECORDROLE_H
#define DBRECORDROLE_H

#include <QString>


/**
 * @brief The DbRecordRole enum Defines roles, a person can represent in database record in DEMoS
 */
enum DbRecordRole
{
    BIR_MAIN,
    BIR_F,
    BIR_M,
    BIR_MIDWIFE,
    BIR_GRANTED,
    BIR_F_F,
    BIR_F_M,
    BIR_F_M_F,
    BIR_F_M_M,
    BIR_M_F,
    BIR_M_M,
    BIR_M_M_F,
    BIR_M_M_M,
    BIR_GF_1,
    BIR_GF_2,
    BIR_GF_3,
    BIR_GF_4,
    BIR_GFREL_1,
    BIR_GFREL_2,
    BIR_GFREL_3,
    BIR_GFREL_4,
    BIR_HUSBAND,
    BIR_BAPT_HUSBAND,
    MAR_GROOM,
    MAR_BRIDE,
    MAR_PRIEST,
    MAR_WIDOWER,
    MAR_G_F,
    MAR_G_M,
    MAR_G_M_F,
    MAR_G_M_M,
    MAR_G_FOST,
    MAR_WIDOW,
    MAR_B_F,
    MAR_B_M,
    MAR_B_M_F,
    MAR_B_M_M,
    MAR_B_FOST,
    MAR_SV_1,
    MAR_SVREL_1,
    MAR_SV_2,
    MAR_SVREL_2,
    MAR_SV_3,
    MAR_SVREL_3,
    MAR_SV_4,
    MAR_SVREL_4,
    MAR_SPEAKER,
    MAR_STARA,
    MAR_BESTMAN,
    MAR_BRIDESMAID,
    BUR_EXAMINATOR,
    BUR_KEEPER,
    BUR_GRAVEDIGGER,
    BUR_MAIN,
    BUR_F,
    BUR_M,
    BUR_M_F,
    BUR_M_M,
    BUR_HUSBAND,
    BUR_WIFE,
    BUR_SON,
    BUR_DAUGHTER,
    BUR_REL1,
    UNDEFINED,
    CHILD,
    PARTNER
};


#endif // DBRECORDROLE_H
