se-sandbox-runner
=================

Qt wrapper for (SELinux) Sandbox

Known Issues:

* In case of sandboxed tasks long startup,
the application may not detect all child processes.
Therefore it's necessary to increase children processes checking timeout.
