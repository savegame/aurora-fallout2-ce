Name:       ru.sashikknox.fallout2ce
Summary:    Fallout 2 CE
Release:    1
Version:    1.0
Group:      Amusements/Games
License:    GPLv2
BuildArch:  %{_arch}
URL:        https://github.com/savegame/lp-public
Source0:    %{name}.tar.gz
# Requires:   SDL2
# Requires:   libGLESv2
# Requires:   dbus
# Requires:   libogg libvorbis
# Requires:   zlib
# Requires:   glib2
# Requires:   libaudioresource

# BuildRequires:   git
BuildRequires:   cmake
BuildRequires:   pkgconfig(audioresource)
BuildRequires:   pkgconfig(sdl2)
# BuildRequires: pulseaudio-devel,  wayland-devel, rsync
# BuildRequires: libGLESv2-devel, wayland-egl-devel
# BuildRequires: wayland-protocols-devel, libusb-devel
# BuildRequires: libxkbcommon-devel, mce-headers, dbus-devel
# BuildRequires: libogg-devel libvorbis-devel

%ifarch armv7hl
%global build_folder build_arm32
%else 
    %ifarch aarch64
        %global build_folder build_arm64
    %else
        %global build_folder build_x86
    %endif
%endif


%description
Fallout 2 CE - open source game engien for Fallout 2 original game

%prep
# prepare
# mkdir -p %{build_folder}
#cd %{build_folder}
# cmake -DCMAKE_BUILD_TYPE=Debug -DAURORAOS=ON ..

%build
mkdir -p %{build_folder}
cd %{build_folder}
cmake -DCMAKE_BUILD_TYPE=Debug -DAURORAOS=ON ..
%make_build 

%install
rm -rf %{buildroot}
cd %{build_folder}
%make_install

%files
%defattr(-,root,root,-)
%attr(755,root,root) %{_bindir}/%{name}
%{_datadir}/icons/hicolor/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/%{name}/qml/*

%changelog 
* Wed Nov 14 2023 sashikknox <sashikknox@gmail.com>
- build for AuroraOS
