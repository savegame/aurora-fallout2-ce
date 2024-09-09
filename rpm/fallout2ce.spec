Name:       ru.sashikknox.fallout2ce
Summary:    Fallout 2 CE
Release:    1
Version:    1.0
Group:      Amusements/Games
License:    GPLv2
URL:        https://github.com/savegame/aurora-fallout2-ce
Source0:    %{name}.tar.gz

%define __provides_exclude_from ^%{_datadir}/%{name}/lib/.*\\.so.*$

BuildRequires: git
BuildRequires: cmake
BuildRequires: pkgconfig(audioresource)
BuildRequires: pkgconfig(glib-2.0)
# BuildRequires: pkgconfig(sdl2)
BuildRequires: pkgconfig(SDL2_image)
BuildRequires: pkgconfig(sailfishsilica)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(mce)
BuildRequires: pkgconfig(wayland-egl)
BuildRequires: pkgconfig(wayland-client)
BuildRequires: pkgconfig(wayland-cursor)
BuildRequires: pkgconfig(wayland-protocols)
BuildRequires: pkgconfig(wayland-scanner)
BuildRequires: pkgconfig(egl)
BuildRequires: pkgconfig(glesv2)
BuildRequires: pkgconfig(xkbcommon)
BuildRequires: pkgconfig(gbm)
BuildRequires: pkgconfig(libpulse)
BuildRequires: patchelf

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
Fallout 2 CE - open source game engine for Fallout 2 original game

%build
echo "Configure SDL2"
cmake \
    -Bbuild_libsdl_%{_arch} \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL_PULSEAUDIO=ON \
    -DSDL_RPATH=OFF \
    -DSDL_STATIC=ON \
    -DSDL_SHARED=OFF \
    -DSDL_WAYLAND=ON \
    -DSDL_X11=OFF \
    -DSDL_DBUS=ON \
    -DSDL_WAYLAND_LIBDECOR=OFF \
    libsdl

pushd build_libsdl_%{_arch}
make -j`nproc`
rsync -avP include-config-*/SDL2/* include/SDL2/
popd 

cmake \
    -B%{build_folder} \
    -DCMAKE_BUILD_TYPE=Debug \
    -DAURORAOS=ON \
    -DLIBSDL_INCLUDE_DIRS="`pwd`/build_libsdl_%{_arch}/include" \
    -DLIBSDL_LDFLAGS="`pwd`/build_libsdl_%{_arch}/libSDL2.a" \
    .

pushd %{build_folder}
%make_build 
popd 

%install
rm -rf %{buildroot}
pushd %{build_folder}
%make_install
popd
#pushd build_libsdl_%{_arch}
#install -m 0655 -D -s libSDL2-2.0.so.0.* -t %{buildroot}%{_datadir}/%{name}/lib/
#popd
#patchelf --force-rpath --set-rpath %{_datadir}/%{name}/lib %{buildroot}%{_bindir}/%{name}

%files
%defattr(-,root,root,-)
%attr(755,root,root) %{_bindir}/%{name}
%{_datadir}/icons/hicolor/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/%{name}

%changelog 
* Wed Nov 14 2023 sashikknox <sashikknox@gmail.com>
- build for AuroraOS

