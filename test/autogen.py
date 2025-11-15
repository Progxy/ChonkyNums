import os
import re
import sys
from datetime import datetime

CTYPE_MAP = {
    "void":                   "None",
    "char":                   "ctypes.c_char",
    "unsigned char":          "ctypes.c_ubyte",
    "short":                  "ctypes.c_short",
    "short int":              "ctypes.c_short",
    "unsigned short":         "ctypes.c_ushort",
    "unsigned short int":     "ctypes.c_ushort",
    "int":                    "ctypes.c_int",
    "unsigned int":           "ctypes.c_uint",
    "long":                   "ctypes.c_long",
    "long int":               "ctypes.c_long",
    "unsigned long int":      "ctypes.c_ulong",
    "unsigned long int":      "ctypes.c_ulong",
    "long long":              "ctypes.c_longlong",
    "long long int":          "ctypes.c_longlong",
    "unsigned long long":     "ctypes.c_ulonglong",
    "unsigned long long int": "ctypes.c_ulonglong",
    "float":                  "ctypes.c_float",
    "double":                 "ctypes.c_double",
    "char *":                 "ctypes.c_char_p",
    "char*":                  "ctypes.c_char_p",
    "void *":                 "ctypes.c_void_p",
    "void*":                  "ctypes.c_void_p",
    "bool":                   "ctypes.c_bool",
    "int8_t":                 "ctypes.c_int8",
    "uint8_t":                "ctypes.c_uint8",
    "int16_t":                "ctypes.c_int16",
    "uint16_t":               "ctypes.c_uint16",
    "int32_t":                "ctypes.c_int32",
    "uint32_t":               "ctypes.c_uint32",
    "int64_t":                "ctypes.c_int64",
    "uint64_t":               "ctypes.c_uint64",
    "s8":                     "ctypes.c_int8",
    "u8":                     "ctypes.c_uint8",
    "s16":                    "ctypes.c_int16",
    "u16":                    "ctypes.c_uint16",
    "s32":                    "ctypes.c_int32",
    "u32":                    "ctypes.c_uint32",
    "s64":                    "ctypes.c_int64",
    "u64":                    "ctypes.c_uint64",
    "s128":                   "ctypes.c_int128",
    "u128":                   "ctypes.c_uint128",
    "union":                  "ctypes.Union"
}

def ctype_from_string(c_type: str, structs_types: [str]) -> str:
    c_type = c_type.replace("const ", "").strip()

    pointer_level = 0
    while c_type.endswith("*"):
        if c_type in ("char*", "char *", "void*", "void *"):
            break
        pointer_level += 1
        c_type = c_type[:-1].strip()

    if c_type in CTYPE_MAP:
        base = CTYPE_MAP[c_type]
    else:
        base = c_type
        if base not in structs_types:
            raise Exception(f"'{c_type}' is not a known type")

    if base in ("ctypes.c_char_p", "ctypes.c_void_p"):
        return base

    for _ in range(pointer_level):
        base = f"ctypes.POINTER({base})"

    return base

def cfile_to_classname(filename: str) -> str:
    # Strip extension (.c, .h, etc.)
    name, _ = os.path.splitext(filename)
    # Replace non-alphanumeric chars (like _ or -) with space
    name = re.sub(r'[^a-zA-Z0-9]', ' ', name)
    # Split into words, capitalize each, and join
    classname = ''.join(word.capitalize() for word in name.split() if word)
    return classname

class Autogen:
    structs_types = []
    anon_cnt = ord('A')

    def __init__(self, c_files, py_file):
        self.c_files = c_files
        self.py_file = py_file
        self.f = open(py_file, "w")
        pass

    def __del__(self):
        try:
            self.f.close()
        except Exception:
            pass

    def parse_function(self, line):
        split_func = line.replace("{", "").replace(")", "").split("(")
        
        function_name = split_func[0].split(" ")[-1]
        function_type = split_func[0].replace(function_name, "").strip()
     
        function_params = split_func[1].split(",")
        for idx, param in enumerate(function_params):
            function_params[idx] = param.strip()

        function_prototype = [function_type, function_name, function_params]
        return function_prototype

    # TODO: Add support for both nested union and structs
    def generate_union(self, lines):
        is_anon = False
        union_buf = ""
        
        union_name = lines[0].split("{")[0].split("union")[-1].split(" ")[-2]
        if (union_name == ""):
            union_name = f"AnonymousUnion{chr(self.anon_cnt)}"
            is_anon = True
            self.anon_cnt += 1

        self.structs_types.append(union_name)
        
        union_buf += f"class {union_name}(ctypes.Union):\n"
        union_buf += "\t_fields_ = [\n"
        
        if len(lines) == 1:
            lines = lines[0].split("{")[-1].split("}")[0].split(",")
        else:
            lines = lines[1:]
            for idx, line in enumerate(lines):
                if "}" in line:
                    lines = lines[:idx]
                    break
        
        for idx, line in enumerate(lines):
            field_name = line.split(" ")[-1].replace(";", "").strip()
            field_type = line.split(field_name)[0].strip()
            field_type = ctype_from_string(field_type, self.structs_types)
            union_buf += f"\t\t(\"{field_name}\", {field_type})"
            
            if idx < len(lines) - 1:
                union_buf += ",\n"
            else:
                union_buf += "\n"

        union_buf += "\t]\n"

        return len(lines), union_name, is_anon, union_buf

    # TODO: Add support for nested structs
    def generate_structure(self, lines):
        anon_unions = ""
        unions_buf = ""

        struct_name = lines[0].split("{")[0].split("struct")[-1].split(" ")[-2]
        self.structs_types.append(struct_name)
        
        struct_buf = ""

        struct_buf += f"class {struct_name}(ctypes.Structure):\n"
        struct_buf += "\t_fields_ = [\n"
        
        if len(lines) == 1:
            lines = lines[0].split("{")[-1].split("}")[0].split(",")
        else:
            lines = lines[1:]
        
        idx = 0
        while idx < len(lines):
            line = lines[idx]
            
            if "}" in line: 
                idx += 1
                continue

            field_name = line.split(" ")[-1].replace(";", "").strip()
            field_type = line.split(field_name)[0].strip()
            field_type = ctype_from_string(field_type, self.structs_types)
            if (field_type == "ctypes.Union"):
                size, union_name, is_anon, union_buf = self.generate_union(lines[idx:])
                idx += size
                
                field_name = union_name
                field_type = union_name
                unions_buf += union_buf

                if is_anon: anon_unions += f"\"{union_name}\","
            
            struct_buf += f"\t\t(\"{field_name}\", {field_type})"
            
            if idx < len(lines) - 1:
                struct_buf += ",\n"
            else:
                struct_buf += "\n"
            idx += 1

        struct_buf += "\t]\n"

        struct_buf += f"\t_anonymous_ = ({anon_unions})\n\n"

        struct_buf += "\tdef __init__(self, **kwargs):\n"
        struct_buf += "\t\tfor field_name, _ in self._fields_:\n"
        struct_buf += "\t\t\tsetattr(self, field_name, 0)\n"
        struct_buf += "\t\tfor key, value in kwargs.items():\n"
        struct_buf += "\t\t\tif key in [f[0] for f in self._fields_]:\n"
        struct_buf += "\t\t\t\tsetattr(self, key, value)\n"
        struct_buf += "\t\t\telse:\n"
        struct_buf += "\t\t\t\traise TypeError(f\"Unknown field: {key}\")\n"
        struct_buf += "\t\tpass\n\n"

        struct_buf += "\tdef __repr__(self):\n"
        struct_buf += "\t\tfields = \"\\n\".join(\n"
        struct_buf += "\t\t\tf\"\\t{name}: {getattr(self, name)!r}\" for name, _ in self._fields_\n"
        struct_buf += "\t\t)\n"
        struct_buf += "\t\treturn f\"{self.__class__.__name__}: {{\\n{fields}\\n}}\"\n\n"

        struct_buf += "\tdef __sizeof__():\n"
        struct_buf += f"\t\treturn ctypes.sizeof({struct_name})\n\n"

        struct_buf += "\tdef to_bytes(self):\n"
        struct_buf += "\t\treturn bytes(ctypes.string_at(ctypes.addressof(self), ctypes.sizeof(self)))\n\n"

        struct_buf += "\tdef hex_dump(self, sep=\" \"):\n"
        struct_buf += "\t\treturn sep.join(f\"{b:02X}\" for b in self.to_bytes())\n\n"

        print(unions_buf, file=self.f)
        print(struct_buf, file=self.f)

        return
    
    def generate_enum(self, lines):
        enum_name = lines[0].split("{")[0].split("enum")[-1].split(" ")[-2]
        self.structs_types.append(enum_name)

        print(f"class {enum_name}(ctypes.c_int):", file=self.f)
        
        if len(lines) == 1:
            lines = lines[0].split("{")[-1].split("}")[0].split(",")
        else:
            lines = lines[1:]
            for line in lines:
                if "}" in line:
                    lines.remove(line)
        
        previous_val = 0
        for idx, line in enumerate(lines):
            field_name = line.strip().split(" ")[0].strip().replace(",", "")
            field_value = line.strip().split("=")
            if len(field_value) == 1:
                previous_val += 1
                field_value = previous_val
            else:
                field_value = int(field_value[-1].replace(",", "").strip(), 0)
                previous_val = field_value
            print(f"\t{field_name} = {field_value}", file=self.f)
        print("\n", end="", file=self.f)

        print("\tdef __repr__(self):", file=self.f)
        print("\t\tfor k, v in self.__class__.__dict__.items():", file=self.f)
        print("\t\t\tif not k.startswith('_') and v == self.value:", file=self.f)
        print("\t\t\t\treturn f\"{self.__class__.__name__}.{k}\"", file=self.f)
        print("\t\treturn f\"{self.__class__.__name__}({self.value})\"\n", file=self.f)

        return

    def generate_function(self, function_prototype, py_class):
        function_type, function_name, function_params = function_prototype
        
        print(f"\tdef {function_name}(self", end="", file=self.f)
        if len(function_params) > 0:
            if not (len(function_params) == 1 and function_params[0] == "void"):
                print(", ", end="", file=self.f)
                for idx, param in enumerate(function_params):
                    param_name = param.split(" ")[-1]
                    print(param_name, end="", file=self.f)
                    if idx < len(function_params) - 1: 
                        print(", ", end="", file=self.f)

        print("):", file=self.f)

        print(f"\t\t{function_name} = self.{py_class}.{function_name}", file=self.f)
        print(f"\t\t{function_name}.argtypes = [ ", end="", file=self.f)

        for idx, param in enumerate(function_params):
            if param == "void":
                print(" ", end="", file=self.f)
            else:
                param = param.split(" ")[:-1]
                param_type = " ".join(param)
                print(ctype_from_string(param_type, self.structs_types), end="", file=self.f)
            if idx < len(function_params) - 1: 
                print(", ", end="", file=self.f)
        print(" ]", file=self.f)
    
        print(f"\t\t{function_name}.restype = {ctype_from_string(function_type, self.structs_types)}", file=self.f)
        print(f"\t\treturn {function_name}(", end="", file=self.f)

        if len(function_params) > 0:
            if not (len(function_params) == 1 and function_params[0] == "void"):
                for idx, param in enumerate(function_params):
                    param_name = param.split(" ")[-1]
                    print(param_name, end="", file=self.f)
                    if idx < len(function_params) - 1:
                        print(", ", end="", file=self.f)
        print(")", end='', file=self.f)

        if ctype_from_string(function_type, self.structs_types) == "ctypes.c_char_p":
            print(".decode()", end='', file=self.f)

        print("\n", file=self.f)
        
        return

    # TODO: Should break this function down, and perform better autogen of classes with methods (OSAS_FUNCTIONS)
    def generate_class(self, c_file):
        with open(c_file, "r") as f:
            lines = f.readlines()

        function_prototypes = []
        for i in range(0, len(lines)):
            line = lines[i]
            if line.startswith("EXPORT_FUNCTION"):
                line = line.replace("EXPORT_FUNCTION", "").strip()
                py_class = py_file.replace(".py", "")
                function_prototypes.append(self.parse_function(line))
            elif line.startswith("EXPORT_STRUCTURE"):
                struct_lines = []
                open_cnt = 0
                close_cnt = 0
                while i < len(lines):
                    struct_lines.append(lines[i])
                    open_cnt += lines[i].count("{")
                    close_cnt += lines[i].count("}")
                    if (open_cnt == close_cnt): break
                    i += 1
                self.generate_structure(struct_lines)
            elif line.startswith("EXPORT_ENUM"):
                enum_lines = []
                while i < len(lines):
                    enum_lines.append(lines[i])
                    if "}" in lines[i]:
                        break
                    i += 1
                self.generate_enum(enum_lines)
        
        if len(function_prototypes) > 0:
            py_class = self.py_file.replace(".py", "")
            py_class = py_class[0].upper() + py_class[1:]
            c_class = cfile_to_classname(c_file.replace(".c", ""))
            print(f"class {c_class}({py_class}):", file=self.f)
            print("\tdef __init__(self):", file=self.f)
            print("\t\tsuper().__init__()", file=self.f)
            py_class = py_file.replace(".py", "")
            print("\t\tpass\n", file=self.f)

            for function_prototype in function_prototypes:
                self.generate_function(function_prototype, py_class)

        return

    def generate_source(self):
        print("# ------------------------------------------------------------", file=self.f)
        print("# THIS FILE IS AUTOGENERATED – DO NOT EDIT MANUALLY", file=self.f)
        print(f"# Generated on {datetime.today().strftime('%d-%m-%Y')} by autogen.py", file=self.f)
        print(f"# From C files: {self.c_files}", file=self.f)
        print("# ------------------------------------------------------------", file=self.f)
        print("import ctypes", file=self.f)
        print("from enum import IntEnum\n", file=self.f)
        
        py_class = self.py_file.replace(".py", "")
        py_class = py_class[0].upper() + py_class[1:]
        print(f"class {py_class}:", file=self.f)
        print("\tdef __init__(self):", file=self.f)
        py_class = self.py_file.replace(".py", "")
        print(f"\t\tself.{py_class} = ctypes.CDLL(\"./{py_class}_py.so\")", file=self.f)
        print("\t\tpass\n", file=self.f)

        for c_file in self.c_files:
            self.generate_class(c_file)
        
        return 

if __name__ == "__main__":
    if len(sys.argv) < 3: 
        print("Usage: autogen.py <c-source> <...> <py-dest>.")
        sys.exit(1)

    c_files = sys.argv[1:-1]
    py_file = sys.argv[-1]
    
    autogen = Autogen(c_files, py_file)

    print(f"Generating {py_file} from {c_files}")
    autogen.generate_source()
    print("Code successfully generated.")

