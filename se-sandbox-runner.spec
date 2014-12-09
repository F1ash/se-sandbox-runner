%global cmake_build_dir build-cmake
%global debug_package %{nil}
%bcond_without qt4
%bcond_without qt5

Name:           se-sandbox-runner
Version:        1.6.12
Release:        3%{?dist}
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
# it has the same binaries with se-sandbox-runner-qt5
Conflicts:      se-sandbox-runner-qt5

%description    qt4
Qt4 wrapper for SELinux Sandbox.
This application runs and controls the configured jobs, running in a sandbox.
Job settings are saved in the application's configuration.

%package        qt5
Summary:        Qt5 wrapper for SELinux Sandbox
# it has the same binaries with se-sandbox-runner-qt4
Conflicts:      se-sandbox-runner-qt4

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
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%doc README.md COPYING Changelog
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop

%changelog
* Wed Dec  3 2014 Fl@sh <kaperang07@gmail.com> - 1.6.12-3
- changed for both qt4 and qt5 building;
- release updated;
- spec %%changelog cleared;
