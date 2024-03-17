
%define debug_package %{nil}

Name:		components-core-5
Version:	1.6.0
Release:	1%{?dist}
Summary:	Extension system core for Phoenix plugin system.
License:	Inteltech
Group:		Development/Other
URL:		http://www.inteltech.ru/
Source0:	%{name}-%{version}.tar.gz
Buildroot:      %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:  gcc

%if 0%{?rhel} == 7
BuildRequires:  qt-devel >= 5.9.0
Requires:       qt	 >= 5.9.0
%endif

%description
Extension system core for Phoenix plugin system.

%package devel
Summary:	Development files for %{name}
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
Provides:	%{name}-devel = %{version}-%{release}

%description devel
Extension system core for Phoenix plugin system.

This package contains the header files needed to develop
applications that use %{name}.

%prep
%setup -q

%if 0%{?rhel} == 7
qmake-qt5 ComponentsCore5alpha.pro
%endif

make

%install
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}
make INSTALL_ROOT=%{buildroot} install

%post
echo /usr/local/%{_lib}/ > /etc/ld.so.conf.d/components_core5.conf
ldconfig

%postun
rm -rf /etc/ld.so.conf.d/components_core5.conf
ldconfig

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)

# libraries
#%{_libdir}/libComponentsCore5alpha.so*
/usr/local/%{_lib}/libComponentsCore5alpha.so*

%files devel
%defattr(-,root,root,-)

# headers
#%{_includedir}/*
/usr/local/include/*


%changelog
* Wed Mar 15 2023 AnthSnow <anthonysnow887@gmail.com> 1.6.0
- Add OS Windows support
- Small bug fixes

* Thu Jan 19 2023 AnthSnow <anthonysnow887@gmail.com> 1.5.0
- Global updates

* Thu Feb 25 2021 AnthSnow <anthonysnow887@gmail.com> 1.1.2
- Added signalInitDone.
- Removed signalStartSuccess, signalStartFailed. 
- Bug fixes and improvements.

* Wed Feb 17 2021 AnthSnow <anthonysnow887@gmail.com> 1.0.0
- Create rpm
