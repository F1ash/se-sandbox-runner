%global cmake_build_dir build-cmake

Name:           se-sandbox-runner
Version:        1.3.5
Release:        1%{?dist}
Summary:        Qt wrapper for SELinux Sandbox
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz 
URL:            https://github.com/F1ash/%{name}

Requires:       xdg-utils
Requires:       policycoreutils-sandbox
Requires:       oxygen-icon-theme
Requires:       hicolor-icon-theme
BuildRequires:  qt4-devel
BuildRequires:  desktop-file-utils
BuildRequires:  cmake

%description
Qt wrapper for SELinux Sandbox.
This application runs and controls the configured jobs, running in a sandbox.
Job settings are saved in the application's configuration.

%prep
%setup -q
mkdir %{cmake_build_dir}

%build
pushd %{cmake_build_dir}
      %cmake ..
      make %{?_smp_mflags}
popd

%install
pushd %{cmake_build_dir}
      %make_install
popd

%check
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%post
/bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

%postun
if [ $1 -eq 0 ] ; then
    /bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    /usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
/usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%files
%doc README.md COPYING Changelog Licenses
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/applications-safety-selinux.png
%{_datadir}/icons/hicolor/*/actions/stop-sandbox.png

%changelog
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
