%global cmake_build_dir build-cmake
%global debug_package %{nil}
%bcond_without qt4
%bcond_without qt5

Name:           se-sandbox-runner
Version:        1.6.14
Release:        1%{?dist}
Summary:        Qt wrapper for SELinux Sandbox
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz 
URL:            https://github.com/F1ash/%{name}

Requires:       xdg-utils
Requires:       policycoreutils-sandbox
%if (0%{?fedora} >= 20)
Requires:       selinux-policy-sandbox
%endif
# for compatibility with the current version
Requires:       sandbox-runner-data >= 0.3.3
Conflicts:      se-sandbox-runner
%if %with qt4
BuildRequires:  qt4-devel
%endif
%if %with qt5
BuildRequires:  qt5-qtbase-devel
%endif
BuildRequires:  desktop-file-utils
BuildRequires:  cmake

%description
Qt wrapper for SELinux Sandbox.
This application runs and controls the configured jobs, running in a sandbox.
Job settings are saved in the application's configuration.

%package        qt4
Summary:        Qt4 wrapper for SELinux Sandbox

%description    qt4
Qt4 wrapper for SELinux Sandbox.
This application runs and controls the configured jobs, running in a sandbox.
Job settings are saved in the application's configuration.

%package        qt5
Summary:        Qt5 wrapper for SELinux Sandbox

%description    qt5
Qt5 wrapper for SELinux Sandbox.
This application runs and controls the configured jobs, running in a sandbox.
Job settings are saved in the application's configuration.

%prep
%setup -q

%build
%if %with qt4
mkdir %{cmake_build_dir}-qt4
pushd %{cmake_build_dir}-qt4
      %cmake ..
      %{make_build}
popd
%endif
%if %with qt5
mkdir %{cmake_build_dir}-qt5
pushd %{cmake_build_dir}-qt5
      %cmake -DBUILD_QT_VERSION=5 ..
      %{make_build}
popd
%endif

%install
%if %with qt4
pushd %{cmake_build_dir}-qt4
      %{make_install}
popd
%endif
%if %with qt5
pushd %{cmake_build_dir}-qt5
      %{make_install}
popd
%endif

%check
%if %with qt4
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}-qt4.desktop
%endif
%if %with qt5
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}-qt5.desktop
%endif

%if %with qt4
%files qt4
%doc README.md Changelog
%license COPYING
%{_bindir}/%{name}-qt4
%{_datadir}/applications/%{name}-qt4.desktop
%endif
%if %with qt5
%files qt5
%doc README.md Changelog
%license COPYING
%{_bindir}/%{name}-qt5
%{_datadir}/applications/%{name}-qt5.desktop
%endif

%changelog
* Fri Nov  6 2015 Fl@sh <kaperang07@gmail.com> - 1.6.14-1
- added %%license field to %%files;
- version updated;

* Tue Feb 10 2015 Fl@sh <kaperang07@gmail.com> - 1.6.13-1
- version updated;

* Sat Dec 13 2014 Fl@sh <kaperang07@gmail.com> - 1.6.12-4
- fixed for both qt4 and qt5 building;
- release updated;
- spec %%changelog cleared;
