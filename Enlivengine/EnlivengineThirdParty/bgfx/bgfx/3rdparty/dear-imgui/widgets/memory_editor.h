namespace ImGui
{
    struct MemoryEditor
    {
        bool    AllowEdits;
        bool    HexII;
        int     Rows;
        int     DataEditingAddr;
        bool    DataEditingTakeFocus;
        char    DataInput[32];
        char    AddrInput[32];

        MemoryEditor()
        {
            AllowEdits = true;
            HexII = true;
            Rows = 16;
            DataEditingAddr = -1;
            DataEditingTakeFocus = false;
#ifdef _MSC_VER
            strcpy_s(DataInput, "");
            strcpy_s(AddrInput, "");
#else
			strcpy(DataInput, "");
			strcpy(AddrInput, "");
#endif // _MSC_VER
        }

        void Draw(void* mem_data, int mem_size, int base_display_addr = 0);
        void Draw(const void* mem_data, int mem_size, int base_display_addr = 0);
    };
} // namespace ImGui
