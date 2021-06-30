project "bzadmin"
  kind "ConsoleApp"
  files { "*.cxx", "*.h", "../../include/*.h" }
  vpaths {
    ["Header Files/include"] = "../../include",
    ["Header Files"] = "**.h",
    ["Source Files"] = "**.cxx"
  }
  defines { "BUILDING_BZADMIN" }
  links { "date", "game", "net", "common", "cares", "curl", "ncurses" }

  filter "system:windows"
    removelinks { "ncurses" }
    links { "pdcurses", "regex", "winmm", "ws2_32", "SDL2" }
    postbuildcommands {
      "if not exist ..\\..\\bin_$(Configuration)_$(Platform) mkdir ..\\..\\bin_$(Configuration)_$(Platform)",
      "copy \"$(OutDir)bzadmin.exe\" ..\\..\\bin_$(Configuration)_$(Platform)\\",
      "copy \"..\\..\\dependencies\\output-windows-$(Configuration)-$(PlatformShortName)\\bin\\*.dll\" ..\\..\\bin_$(Configuration)_$(Platform)\\"
    }
  filter { "system:windows", "configurations:Release" }
    removelinks "curl"
    links "libcurl"
  filter { "system:windows", "configurations:Debug" }
    removelinks { "cares", "curl" }
    links { "caresd", "libcurl_debug" }

  filter "system:macosx"
    links { "Cocoa.framework", "resolv" }