import csv

# Constants

CAN_ID_CSV_FILENAME = "FEB_CAN_ID.csv"
CAN_STATIC_ID_CSV_FILENAME = "FEB_CAN_Static_ID.csv"

C_HEADER_FILENAME = "FEB_CAN_ID.h"
C_HEADER_GUARD = "INC_FEB_CAN_ID_H_"
C_CAN_MESSAGE_PREFIX = "FEB_CAN_ID_"

PYTHON_FILENAME = "feb_can_id.py"

# Global Variables

CAN_Static_Id_Set = set()
CAN_Id = 0

# CAN

def Get_Available_CAN_Id() -> int:
    global CAN_Id
    while CAN_Id in CAN_Static_Id_Set:
        CAN_Id += 1
    CAN_Id += 1
    return CAN_Id - 1

def Format_CAN_Message_Name(name: str) -> str:
    return C_CAN_MESSAGE_PREFIX + "_".join(name.upper().split(" "))
    
# C Header File

def Is_C_Comment(csv_row: "list(str)") -> bool:
    return "//" in csv_row[0]

def C_Header_Comment(s: str) -> str:
    return f"// {'*' * 40} {s} {'*' * 40}"

def C_Define_Macro(name: str, value: "hex") -> str:
    return f"#define {name} {value}"

def Generate_C_Header_File(filename: str, CAN_Id_Data: list[dict], CAN_Static_Id_Data: list[dict]) -> None:
    def Write_CAN_ID_To_File(CAN_Id_Data: list[dict]):
        for CAN_Message in CAN_Id_Data:
            if "comment" in CAN_Message:
                C_Header_File.write("\n")
                C_Header_File.write(CAN_Message["comment"] + "\n")
            else:
                C_Header_File.write(C_Define_Macro(CAN_Message["name"], CAN_Message["id"]) + "\n")

    with open(filename, "w") as C_Header_File:
        # Open header guard
        C_Header_File.write(f"#ifndef {C_HEADER_GUARD}\n")
        C_Header_File.write(f"#define {C_HEADER_GUARD}\n")

        # Write CAN IDs
        C_Header_File.write("\n")
        C_Header_File.write(C_Header_Comment("Static CAN IDs") + "\n")
        Write_CAN_ID_To_File(CAN_Static_Id_Data)

        C_Header_File.write("\n")
        C_Header_File.write(C_Header_Comment("Dynamic CAN IDs") + "\n")
        Write_CAN_ID_To_File(CAN_Id_Data)

        # Close header guard
        C_Header_File.write("\n")
        C_Header_File.write(f"#endif /* {C_HEADER_GUARD} */\n")

# Python file

def Python_Header_Comment(s: str) -> str:
    return f"# {'*' * 40} {s} {'*' * 40}"

def Python_Assign_Macro(name: str, value: "hex") -> str:
    return f"{name} = {value}"

def Generate_Python_File(filename: str, CAN_Id_Data: list[dict], CAN_Static_Id_Data: list[dict]) -> None:
    def Write_CAN_ID_To_File(CAN_Id_Data: list[dict]):
        for CAN_Message in CAN_Id_Data:
            if "comment" in CAN_Message:
                Python_File.write("\n")
                Python_File.write(f"# {CAN_Message['comment'][3:]}\n")
            else:
                Python_File.write(Python_Assign_Macro(CAN_Message["name"].replace(C_CAN_MESSAGE_PREFIX, ''), 
                                                      CAN_Message["id"]) + "\n")

    with open(filename, "w") as Python_File:
        # Static CAN IDs
        Python_File.write(Python_Header_Comment("Static CAN IDs") + "\n")
        Write_CAN_ID_To_File(CAN_Static_Id_Data)
        Python_File.write("\n")

        # Dynamic CAN IDs
        Python_File.write(Python_Header_Comment("Dynamic CAN IDs") + "\n")
        Write_CAN_ID_To_File(CAN_Id_Data)

# CSV File

def Is_CSV_Comment(CSV_Row: "list(str)") -> bool:
    return "#" in CSV_Row[0]

def Read_CSV_Data(filename: str, Row_Func: "function") -> "list(str)":
    CAN_Id_Data = []
    with open(filename) as CSV_File:
        CSV_Reader = csv.reader(CSV_File, delimiter=",")
        for row in CSV_Reader:
            if row and not Is_CSV_Comment(row):
                CAN_Id_Data.append(Row_Func(row))
    return CAN_Id_Data

def Process_CAN_Id_CSV_Row(row: "list(str)") -> dict:
    if Is_C_Comment(row):
        CAN_Message_Data = {"comment": row[0]}
    else:
        CAN_Message_Data = {
            "name": Format_CAN_Message_Name(row[0]),
            "id": hex(Get_Available_CAN_Id())
        }
    return CAN_Message_Data

def Process_CAN_Static_Id_CSV_Row(row: "list(str)") -> dict:
    if Is_C_Comment(row):
        CAN_Message_Data = {"comment": row[0]}
    else:
        CAN_Id = int(row[1], 0)
        CAN_Message_Data = {
            "name": Format_CAN_Message_Name(row[0]),
            "id": hex(CAN_Id)
        }
        CAN_Static_Id_Set.add(CAN_Id)
    return CAN_Message_Data

def main():
    CSV_Static_Id_Data = Read_CSV_Data(CAN_STATIC_ID_CSV_FILENAME, Process_CAN_Static_Id_CSV_Row)
    CSV_ID_Data = Read_CSV_Data(CAN_ID_CSV_FILENAME, Process_CAN_Id_CSV_Row)
    Generate_C_Header_File(C_HEADER_FILENAME, CSV_ID_Data, CSV_Static_Id_Data)
    Generate_Python_File(PYTHON_FILENAME, CSV_ID_Data, CSV_Static_Id_Data)

if __name__ == "__main__":
    main()
