# Notify Me

Small Windows executable which just flashes the WindowsTerminal.exe window 5 times.

Useful when added to the end of long running jobs.

E.g:

* Alias/rename to `nt`

```
terraform apply; nt
```

Will flash the WindowsTerminal.exe window once terraform finishes.

Note:
* Using `;` instead of `&&` means that you'll be notified regardless of if the previous process finish successfully
* You can run executables on WSL in order to ping the WindowsTerminal
* Change the count or program that flashes in the main method if desired
