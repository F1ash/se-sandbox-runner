se-sandbox-runner
=================

Qt wrapper for (SELinux) Sandbox

Known Issues:

* In case of sandboxed tasks long startup,
the application may not detect all child processes.
Therefore it's necessary to increase children processes checking timeout.

* Since Fedora 18 for complete application operating
you need to enable Sandbox Policy by executing the following as root :
semodule -e sandbox

* If a Job has Shred enabled and you kill this running Job, then
sandbox utility is not completed correctly, however the application tries
to clear the used directories.
