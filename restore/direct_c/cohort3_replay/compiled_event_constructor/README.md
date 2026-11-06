Existing event-manager constructor: two fills, six native/C routes.

Actual native vector constructors and actual C initialization of all150 records
execute without controlled endpoints. Receiver guards, final full EAX=self,
nonvolatiles, stack cleanup, x87 state and restored FS0 are compared.
Only the existing typed C function is retained; no new algorithm is introduced.
Use run.py --run for offline comparison. Exception/unwind and arbitrary receiver
alias domains remain pending. Complete gameplay equivalence is not established.
