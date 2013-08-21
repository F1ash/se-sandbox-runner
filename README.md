se-sandbox-runner
=================

Qt-wrap for (SELinux) Sandbox

Known Issues:

* In case of sandboxed tasks long startup,
the application may not detect all child processes.
Therefore it's necessary to increase children processes checking timeout.

* Since Fedora 18 for complete application operating
you need to enable Sandbox Policy by executing the following as root
# semodule -e sandbox
