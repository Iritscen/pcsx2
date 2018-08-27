/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2018  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#ifndef __SAVESLOTS_H__
#define __SAVESLOTS_H__

#include "PS2Edefs.h"
#include "System.h"
#include "Elfheader.h"

class Saveslot
{
protected:
public:
    int slot_num;
    bool empty;
    wxDateTime updated;
    wxString note;
    int crc;

    Saveslot()
    {
        slot_num = 0;
        empty = true;
        updated = wxInvalidDateTime;
        note = "";
        crc = ElfCRC;
    }

    Saveslot(int i)
    {
        slot_num = i;
        empty = true;
        updated = wxInvalidDateTime;
        note = "";
        crc = ElfCRC;
    }

    ~Saveslot() {}

    __forceinline bool isUsed()
    {
        if (ElfCRC == 0)
            return false;
        else
            return wxFileExists(SaveStateBase::GetFilename(slot_num));
    }

    __forceinline wxDateTime GetTimestamp()
    {
        if (isUsed()) {
            return wxDateTime(wxFileModificationTime(SaveStateBase::GetFilename(slot_num)));
        } else {
            return wxInvalidDateTime;
        }
    }

    __forceinline void UpdateCache()
    {
        empty = !isUsed();
        updated = GetTimestamp();
        crc = ElfCRC;
    }

    __forceinline void ConsoleDump()
    {
        Console.WriteLn("Slot %i information:", slot_num);
        Console.WriteLn("Internal CRC = %i; Current CRC = %i.", crc, ElfCRC);
        if (empty)
            Console.WriteLn("Slot cache says it is empty.");
        else
            Console.WriteLn("Slot cache says it is used.");
        Console.WriteLn("Write time is %s %s.", updated.FormatDate(), updated.FormatTime());

        if (isUsed())
            Console.WriteLn("The disk has a file on it dated %s %s.", GetTimestamp().FormatDate(), GetTimestamp().FormatTime());
    }
};

extern Saveslot saveslot_cache[10];

//extern void States_registerLoadBackupMenuItem( wxMenuItem* loadBackupMenuItem );

extern void States_DefrostCurrentSlotBackup();
extern void States_DefrostCurrentSlot();
extern void States_FreezeCurrentSlot();
extern void States_CycleSlotForward();
extern void States_CycleSlotBackward();

extern void Saveslots_UpdateFromDisk();
extern void States_SetCurrentSlot(int slot);
extern int States_GetCurrentSlot();

#endif