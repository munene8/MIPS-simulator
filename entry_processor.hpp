#ifndef ENTRY_PROCESSOR_HPP
#define ENTRY_PROCESSOR_HPP

#include "include_define.hpp"

using namespace std;

class Entry_Processor {
    friend class MIPS_Simulator;
private:
    map<string, int> registerIdx, labelNameIdx;
    map<int, int> labelIdxAddress;
    
    void RegisterIdxInit() {
        registerIdx["$0"] = 0;
        registerIdx["$1"] = 1;
        registerIdx["$2"] = 2;
        registerIdx["$3"] = 3;
        registerIdx["$4"] = 4;
        registerIdx["$5"] = 5;
        registerIdx["$6"] = 6;
        registerIdx["$7"] = 7;
        registerIdx["$8"] = 8;
        registerIdx["$9"] = 9;
        registerIdx["$10"] = 10;
        registerIdx["$11"] = 11;
        registerIdx["$12"] = 12;
        registerIdx["$13"] = 13;
        registerIdx["$14"] = 14;
        registerIdx["$15"] = 15;
        registerIdx["$16"] = 16;
        registerIdx["$17"] = 17;
        registerIdx["$18"] = 18;
        registerIdx["$19"] = 19;
        registerIdx["$20"] = 20;
        registerIdx["$21"] = 21;
        registerIdx["$22"] = 22;
        registerIdx["$23"] = 23;
        registerIdx["$24"] = 24;
        registerIdx["$25"] = 25;
        registerIdx["$26"] = 26;
        registerIdx["$27"] = 27;
        registerIdx["$28"] = 28;
        registerIdx["$29"] = 29;
        registerIdx["$30"] = 30;
        registerIdx["$31"] = 31;
    
        registerIdx["$zero"] = 0;
        registerIdx["$at"] = 1;
        registerIdx["$v0"] = 2;
        registerIdx["$v1"] = 3;
        registerIdx["$a0"] = 4;
        registerIdx["$a1"] = 5;
        registerIdx["$a2"] = 6;
        registerIdx["$a3"] = 7;
        registerIdx["$t0"] = 8;
        registerIdx["$t1"] = 9;
        registerIdx["$t2"] = 10;
        registerIdx["$t3"] = 11;
        registerIdx["$t4"] = 12;
        registerIdx["$t5"] = 13;
        registerIdx["$t6"] = 14;
        registerIdx["$t7"] = 15;
        registerIdx["$s0"] = 16;
        registerIdx["$s1"] = 17;
        registerIdx["$s2"] = 18;
        registerIdx["$s3"] = 19;
        registerIdx["$s4"] = 20;
        registerIdx["$s5"] = 21;
        registerIdx["$s6"] = 22;
        registerIdx["$s7"] = 23;
        registerIdx["$t8"] = 24;
        registerIdx["$t9"] = 25;
        registerIdx["$k0"] = 26;
        registerIdx["$k1"] = 27;
        registerIdx["$gp"] = 28;
        registerIdx["$sp"] = 29;
        registerIdx["$fp"] = 30;
        registerIdx["$s8"] = 30;
        registerIdx["$ra"] = 31;
    
        registerIdx["$lo"] = 32;
        registerIdx["$hi"] = 33;
    }
    
    bool ContainBracket(const string &str) {
        for (auto c : str) {
            if (c == '(' || c == ')')
                return true;
        }
        return false;
    }
    
    string GetStringBeforeBracket(const string &str) {
        size_t pos = 0;
        while (str[pos] != '(' && pos < str.size()) ++pos;
        return string(str, 0, pos);
    }
    
    string GetStringBetweenBracket(const string &str) {
        size_t pos1 = 0, pos2 = 0;
        while (str[pos1] != '(' && pos1 < str.size()) ++pos1;
        while (str[pos2] != ')' && pos2 < str.size()) ++pos2;
        return string(str, pos1 + 1, pos2 - pos1 - 1);
    }
    
    Instruction EntryToInstruction(const Entry &entry) {
        Instruction ret;
        ret.op = byte(entry.tokenType);
        // the order of the following if-switches cannot be changed
        if (InClosedInterval(entry.tokenType, _mul, _divu) && entry.argv.size() == 2) {
            ret.rs = registerIdx[entry.argv[0]];
            ret.rt = registerIdx[entry.argv[1]];
        }
        else if (InClosedInterval(entry.tokenType, _neg, _negu) || entry.tokenType == _move) {
            ret.rd = registerIdx[entry.argv[0]];
            ret.rs = registerIdx[entry.argv[1]];
        }
        else if (entry.tokenType == _li) {
            ret.rd = registerIdx[entry.argv[0]];
            ret.constant = StringToInteger(entry.argv[1]);
        }
        else if (entry.tokenType == _b || entry.tokenType == _j || entry.tokenType == _jal) {
            ret.address = labelIdxAddress[labelNameIdx[entry.argv[0]]];
        }
        else if (InClosedInterval(entry.tokenType, _add, _sne)) {
            ret.rd = registerIdx[entry.argv[0]];
            ret.rs = registerIdx[entry.argv[1]];
            if (entry.argv[2].at(0) == '$')
                ret.rt = registerIdx[entry.argv[2]];
            else
                ret.constant = StringToInteger(entry.argv[2]);
        }
        else if (InClosedInterval(entry.tokenType, _beq, _blt)) {
            ret.rs = registerIdx[entry.argv[0]];
            ret.address = labelIdxAddress[labelNameIdx[entry.argv[2]]];
            if (entry.argv[1].at(0) == '$')
                ret.rt = registerIdx[entry.argv[1]];
            else
                ret.constant = StringToInteger(entry.argv[1]);
        }
        else if (InClosedInterval(entry.tokenType, _beqz, _bltz)) {
            ret.rs = registerIdx[entry.argv[0]];
            ret.address = labelIdxAddress[labelNameIdx[entry.argv[1]]];
        }
        else if (entry.tokenType == _jr || entry.tokenType == _jalr) {
            ret.rs = registerIdx[entry.argv[0]];
        }
        else if (InClosedInterval(entry.tokenType, _la, _lw)) {
            ret.rd = registerIdx[entry.argv[0]];
            if (ContainBracket(entry.argv[1])) {
                ret.rt = registerIdx[GetStringBetweenBracket(entry.argv[1])];
                ret.offset = StringToInteger(GetStringBeforeBracket(entry.argv[1]));
            }
            else {
                ret.address = labelIdxAddress[labelNameIdx[entry.argv[1]]];
            }
        }
        else if (InClosedInterval(entry.tokenType, _sb, _sw)) {
            ret.rs = registerIdx[entry.argv[0]];
            if (ContainBracket(entry.argv[1])) {
                ret.rt = registerIdx[GetStringBetweenBracket(entry.argv[1])];
                ret.offset = StringToInteger(GetStringBeforeBracket(entry.argv[1]));
            }
            else {
                ret.address = labelIdxAddress[labelNameIdx[entry.argv[1]]];
            }
        }
        else if (entry.tokenType == _mfhi || entry.tokenType == _mflo) {
            ret.rd = registerIdx[entry.argv[0]];
        }
        else {
            if (entry.tokenType != _nop && entry.tokenType != _syscall) {
#ifdef DEBUG_ENTRY_TO_INSTRUCTION
                cout << "Invalid tokenType, cannot be converted to instruction." << endl;
#endif                
            }
        }
#ifdef DEBUG_ENTRY_TO_INSTRUCTION
        cout << "Instruction: [" << int(ret.op) << ", " << int(ret.rs) \
             << ", " << int(ret.rt) << ", " << int(ret.rd) << ", " << int(ret.constant) \
             << ", " << int(ret.offset) << ", " << int(ret.address) << "]" << endl; 
#endif
        return ret;
    }
    
    
    void ProcessDataEntry(const Entry &entry, byte* memorySpace, int &staticDataMemoryTop) {
        int ele;
        switch (entry.tokenType) {
        case _align:
            ele = 1 << StringToInteger(entry.argv[0]);
            while (staticDataMemoryTop % ele != 0 && staticDataMemoryTop < maxMemoryByte) 
                memorySpace[staticDataMemoryTop++] = byte(0);    
            break;
        case _ascii:
            for (size_t i = 0; i < entry.argv[0].length(); ++i) {
                *(reinterpret_cast<char*>(memorySpace + staticDataMemoryTop)) = \
                    entry.argv[0].at(i);
                staticDataMemoryTop += sizeof(char);            
            }
            break;
        case _asciiz:
            for (size_t i = 0; i < entry.argv[0].length(); ++i) {
                *(reinterpret_cast<char*>(memorySpace + staticDataMemoryTop)) = \
                    entry.argv[0].at(i);
                staticDataMemoryTop += sizeof(char);            
            }
            *(reinterpret_cast<char*>(memorySpace + staticDataMemoryTop)) = '\0';
            staticDataMemoryTop += sizeof(char);
            break;
        case _byte:
        case _half:
        case _word:
            for (size_t i = 0; i < entry.argv.size(); ++i) {
                if (entry.argv[i].at(0) == '\'') 
                    *(reinterpret_cast<char*>(memorySpace + staticDataMemoryTop)) = \
                        DecodeEscapedString(string(entry.argv[i], 1, entry.argv[i].length() - 2)).at(0);
                else
                    *(reinterpret_cast<int*>(memorySpace + staticDataMemoryTop)) = \
                        StringToInteger(entry.argv[i]);
                switch (entry.tokenType) {
                case _byte:
                    staticDataMemoryTop += 1;
                    break;
                case _half:
                    staticDataMemoryTop += 2;
                    break;
                case _word:
                    staticDataMemoryTop += 4;
                    break;
                default: break;
                }
            }
            break;
        case _space:
            staticDataMemoryTop += StringToInteger(entry.argv[0]);
            break;
        default: break;
        }
    }
    
    Entry_Processor() = default;
    Entry_Processor(Entry_Processor const&);
    Entry_Processor& operator=(Entry_Processor const&);
public:
    static Entry_Processor& instance() {
        static Entry_Processor ins;
        return ins;
    }

    void ProcessEntries(const vector<Entry> &entries, byte* memorySpace, int &textMemoryTop, int &staticDataMemoryTop, int &dynamicDataMemoryTop) {
        // get instruction address
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].tokenType == _label) {
                labelNameIdx[entries[i].labelName] = entries[i].idx;
                continue;
            }
            if (entries[i].entryType != dotText) continue;
            labelIdxAddress[entries[i].idx] = textMemoryTop;
            textMemoryTop += sizeof(Instruction);
        }
        staticDataMemoryTop = textMemoryTop;
        // process static data
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].entryType != dotData || entries[i].tokenType == _label)
                continue;
            labelIdxAddress[entries[i].idx] = staticDataMemoryTop;
            ProcessDataEntry(entries[i], memorySpace, staticDataMemoryTop);
        }
        dynamicDataMemoryTop = staticDataMemoryTop;
        textMemoryTop = 0;
        // process instructions
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].entryType != dotText || entries[i].tokenType == _label)
                continue;
            *(reinterpret_cast<Instruction*>(memorySpace + textMemoryTop)) = EntryToInstruction(entries[i]);
        }
    }
};

#endif // ENTRY_PROCESSOR_HPP
