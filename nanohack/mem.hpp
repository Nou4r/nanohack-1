namespace mem {
	uintptr_t find_pattern(uintptr_t base, const PBYTE Pattern, const char* Mask) {
		PBYTE Start = (PBYTE)base;
		PIMAGE_NT_HEADERS NTHead = (PIMAGE_NT_HEADERS)(Start + ((PIMAGE_DOS_HEADER)Start)->e_lfanew);
		DWORD Len = NTHead->OptionalHeader.SizeOfImage;

		for (PBYTE region_it = Start; region_it < (Start + Len); ++region_it) {
			if (*region_it == *Pattern) {
				bool found = true;
				const unsigned char* pattern_it = Pattern, * mask_it = (const PBYTE)Mask, * memory_it = region_it;
				for (; *mask_it && (memory_it < (Start + Len)); ++mask_it, ++pattern_it, ++memory_it) {
					if (*mask_it != 'x') continue;
					if (*memory_it != *pattern_it) {
						found = false;
						break;
					}
				}

				if (found)
					return (uintptr_t)region_it;
			}
		}

		return 0;
	}
	template<typename T = uintptr_t>
	T read_chain(uintptr_t start_address, std::vector<uintptr_t> addresses) {
		T ret = 0;
		for (uintptr_t addy : addresses) {
			ret = *reinterpret_cast<T*>(start_address + addy);
		}
		return ret;
	}
}