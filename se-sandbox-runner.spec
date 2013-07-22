%define cmake_build_dir build-cmake

Name: se-sandbox-runner
Version: 1.0
Release: 1%{?dist}
Summary: Qt-wrap for SELinux Sandbox
Group: Applications/System
License: GPLv2+
Source0: https://github.com/F1ash/%{name}/archive/%{version}.tar.gz
URL: https://github.com/F1ash/%{name}

Requires: Qt4 oxygen-icon-theme
BuildRequires: gcc-c++ qt4-devel desktop-file-utils

%description
%{name}
Qt-wrap for SELinux Sandbox.
App run and control the configured jobs, running into sandbox.
Sett


%prep
%setup -q
mkdir %{cmake_build_dir}
pushd %{cmake_build_dir}
      %cmake ..
popd

%build
pushd %{cmake_build_dir}
      make %{?_smp_mflags}
popd

%install
pushd %{cmake_build_dir}
      make install DESTDIR=%{buildroot}/%{_prefix}
popd

desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/256x256/apps/applications-safety.png
%doc README.md COPYING Changelog Licenses

%post
/bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

%postun
if [ $1 -eq 0 ] ; then
    /bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    /usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
/usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%changelog
* Sat Jul 20 2013 Fl@sh <kaperang07@gmail.com> - 1.0-1
- Initial build
