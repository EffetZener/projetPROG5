/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include "registers.h"

int arm_load_store(arm_core p, uint32_t ins) {
    int B, H, I, L, P, S, U, W;
    int action, index, adresse;
    int Rm, Rd, Rn;    
    
    int shift_imm;
    int shift;
    
    uint32_t value_word;
    uint16_t value_half;
    uint8_t value_byte;
    uint8_t offset_8;
   
    Rd = (ins >> 12) & 0b1111;
    Rn = (ins >> 16) & 0b1111;
    W = (ins >> 21) & 1;
    U = (ins >> 23) & 1;
    P = (ins >> 24) & 1;
    action = ins >> 25 & 0b111;
    if (action == 0) // demi et double mots
    {
        S = (ins >> 6) & 1;
        H = (ins >> 5) & 1;
        L = (ins >> 20) & 1;
        I = (ins >> 22) & 1;

        if (I == 1)
        {
            offset_8 = ((ins >> 8) << 4) | (ins & 0b1111);
        }
        else
        {
            offset_8 = ins & 0b1111; // offset au lie de Rm
        }
        
        if (P == 0)
        {
            if (W == 1)
            {
                return UNDEFINED_INSTRUCTION; //UNPREDICTABLE;
            }
        }
        
        if (U == 1)
        {
            adresse = arm_read_register(p, Rn) + offset_8;
        }
        else
        {
            adresse = arm_read_register(p, Rn) - offset_8;
        }
        
        if (L == 0 && S == 0 && H == 1)
        {
            //store halfword
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            value_half = arm_read_register(p, Rd);
            arm_write_half(p, arm_read_register(p, Rn), value_half);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else if(L == 0 && S == 1 && H == 0 )
        {
            //load double
            // Non pris en compte. #TODO
        }
        else if (L == 0 && S == 1 && H == 1)
        {
            //store double
            // Non pris en compte. #TODO
        }
        else if (L == 1 && S == 0 && H == 1)
        {
            //load halfword unsigned
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            arm_read_half(p,arm_read_register(p, Rn), &value_half);
            arm_write_register(p, Rd, value_half);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else if (L == 1 && S == 1 && H == 0)
        {
            //load byte signed
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            arm_read_byte(p,arm_read_register(p, Rn), &value_byte);
            arm_write_register(p, Rd, value_byte);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else if (L == 1 && S == 1 && H == 1)
        {
            //load halfword signed
            if (P == 1 && W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            arm_read_half(p,arm_read_register(p, Rn), &value_half);
            arm_write_register(p, Rd, value_byte);
            if (P ==0 && W == 0)
            {
                arm_write_register(p, Rn, adresse);
            }
        }
        else
        {
            //couille
        }
       
        if (P == 1)
        {
            if (W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
        }

    }
    else
    {
        L = (ins >> 20) & 1;
        B = (ins >> 22) & 1;
        I = (ins >> 25) & 1;

        if (I == 1)
        {
            Rm = ins & 0b1111;
            shift_imm = (ins >> 7 ) &0b11111;
            shift = (ins >> 5) & 0b11;
            //Alors:
            //Traitement pour un mot
            if (shift == 0b00)
            {
                //LSL (declage a gauche)
                index = arm_read_register(p, Rm) << shift_imm;
            }
            else if (shift == 0b01)
            {
                //LSR (decalage a droite)
                if (shift_imm == 0)
                {
                    index = 0;
                }
                else
                {
                    index = arm_read_register(p, Rm) >> shift_imm;
                }
            }
            else if (shift == 0b10)
            {
                //ASR (decalage arithmetique a droite)
                if (shift_imm == 0)
                {
                    if (((Rm >> 31) & 1) == 1)
                    {
                        index = 0xFFFFFFFF;
                    }
                    else
                    {
                        index = 0;
                    }
                }
                else
                {
                    index = arm_read_register(p, Rm) >> shift_imm;
                }
                
            }
            else if (shift == 0b11)
            {
                //ROR ou RRX
                if (shift_imm == 0)
                {
                    index = ((arm_read_register(p, CPSR) >> 29) &0b1) || Rm >> 1;
                }
                else
                {
                    index = (arm_read_register(p, Rm) >> shift_imm) | (arm_read_register(p, Rm) << (32-shift_imm));
                }                
            }
        }
        else // I == 0
        {
            //Si non :
            //On recupere le offset dans index
            index = ins & 0xFFF;
        }

        if (U == 1)
        {
            adresse = arm_read_register(p, Rn) + index;
        }
        else
        {
            adresse = arm_read_register(p, Rn) - index;
        }
        
        if (P == 0)
        {  
            if (W == 0)
            {
                if (L == 1)
                {
                    if (B == 1) // Load unsigned byte
                    {
                        arm_read_byte(p, arm_read_register(p, Rn), &value_byte);
                        arm_write_register(p, Rd, value_byte);
                    }
                    else       //Load word
                    {
                        arm_read_word(p, arm_read_register(p, Rn), &value_word);
                        arm_write_register(p, Rd, value_word);
                    }
                }
                else
                {
                    if (B == 1) // Store unsigned byte
                    {
                        value_byte = arm_read_register(p, Rd);
                        arm_write_byte(p, arm_read_register(p, Rn), value_byte);
                    }
                    else        // Store word
                    {
                        value_word = arm_read_register(p, Rd);
                        arm_write_word(p, arm_read_register(p, Rn), value_word);
                    }
                }
            }
            else // W == 1
            {
                if (L == 1)
                {
                    if (B == 1) //Load unsigned byte
                    {
                        arm_read_byte(p, arm_read_register(p, Rn), &value_byte);
                        arm_write_usr_register(p, Rd, value_byte);
                    }
                    else
                    {
                        //Load word
                        arm_read_word(p, arm_read_register(p, Rn), &value_word);
                        arm_write_usr_register(p, Rd, value_word);
                    }
                }
                else
                {
                    if (B == 1) // Store unsigned byte
                    {
                        value_byte = arm_read_usr_register(p, Rd);
                        arm_write_byte(p, arm_read_register(p, Rn), value_byte);
                    }
                    else        //Store word
                    {
                        value_word = arm_read_usr_register(p, Rd);
                        arm_write_word(p, Rn, value_word);
                    }
                }
            }
            arm_write_register(p, Rn, adresse);
        }
        else // P == 1
        {
            if (W == 1)
            {
                arm_write_register(p, Rn, adresse);
            }
            if (L == 1)
            {
                //Load word
                arm_read_word(p, adresse, &value_word);
                arm_write_register(p, Rd, value_word);
            }
            else
            {
                //Store word
                value_word = arm_read_register(p, Rd);
                arm_write_word(p, adresse, value_word);
            }
        }
    }
    
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
