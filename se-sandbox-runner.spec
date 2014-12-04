%global cmake_build_dir build-cmake
%global debug_package %{nil}
%bcond_without qt4
%bcond_without qt5

Name:           se-sandbox-runner
Version:        1.6.12
Release:        2%{?dist}
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
* Wed Dec  3 2014 Fl@sh <kaperang07@gmail.com> - 1.6.12-2
- changed for both qt4 and qt5 building;
- release updated;

* Tue Jul  8 2014 Fl@sh <kaperang07@gmail.com> - 1.6.12-1
- version updated;

* Thu Feb 13 2014 Fl@sh <kaperang07@gmail.com> - 1.6.10-1
- changed the selinux-policy-sandbox version in R;
- version updated;

* Sat Feb  8 2014 Fl@sh <kaperang07@gmail.com> - 1.6.8-2
- added a condition to the selinux-policy-sandbox requirement;
- release updated;

* Fri Feb  7 2014 Fl@sh <kaperang07@gmail.com> - 1.6.8-1
- added selinux-policy-sandbox to R;
- version updated;

* Wed Jan 22 2014 Fl@sh <kaperang07@gmail.com> - 1.4.7-1
- added sandbox-runner-data to R;
- removed hicolor-icon-theme, oxygen-icon-theme from R;
- removed Icon_Cache ScriptletSnippets;
- removed unnecessary %%files path;
- version updated;

* Fri Dec 27 2013 Fl@sh <kaperang07@gmail.com> - 1.3.5-1
- added new icon path & Icon_Cache ScriptletSnippets;
- added hicolor-icon-theme to R;
- version updated;

* Tue Nov 26 2013 Fl@sh <kaperang07@gmail.com> - 1.3.4-1
- version updated;

* Sat Sep 14 2013 Fl@sh <kaperang07@gmail.com> - 1.3.3-2
- release updated;

* Thu Sep 12 2013 Fl@sh <kaperang07@gmail.com> - 1.3.3-1
- version updated;
- spec simplified :
- Application's Icon directory changed to %%{_datadir}/pixmaps;
- removed hicolor-icon-theme from R;
- removed Icon_Cache ScriptletSnippets;
- returned back the short Source0 path

* Fri Sep  6 2013 Igor Gnatenko <i.gnatenko.brain@gmail.com> - 1.3.2-2
- move desktop-file-validate to %%check section
- use %%make_install tag instead of make install DESTDIR=%%{buildroot}
- remove gcc-c++ from BR
- %%global instead of %%define
- remove qt4 from R
- add hicolor-icon-theme as R

* Mon Sep  2 2013 Fl@sh <kaperang07@gmail.com> - 1.3.2-1
- version updated

* Sat Aug 24 2013 Fl@sh <kaperang07@gmail.com> - 1.2.1-1
- some fixes

* Fri Aug 23 2013 Fl@sh <kaperang07@gmail.com> - 1.2-2
- fixed description;
- release updated

* Thu Aug 22 2013 Fl@sh <kaperang07@gmail.com> - 1.2-1
- version updated;

* Sat Jul 20 2013 Fl@sh <kaperang07@gmail.com> - 1.0-1
- Initial build
