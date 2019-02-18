# Security Overview

This document is a very high overview related to the security of Hush, with links to other resources.

## SECURITY AUDIT

Hush itself has not had a 3rd party code audit, but our upstream fork, Zcash, has. More details on that here:

https://z.cash/blog/audit-results.html

# KNOWN SECURITY ISSUES

Each release contains a `./doc/security-warnings.md` document describing security
issues known to affect that release. You can find the most recent version of
this document [here](https://github.com/MyHush/hush/blob/master/doc/security-warnings.md)

Note that this link points to the "in development" version of the file, so it
may have more recent findings than the version released with your software. (It
might also have issues that are only relevant for the upcoming release which
don't affect the current release or older software.)

# What if myhush.org get hacked?

In the event the Hush website is down or hacked, please also check these
twitter handles: @dukeleto and @MyHushTeam. The Hush protocol has an
alert system and currently a small set of people control the keys to issue
alerts. These will be sent to all nodes, if necessary in an emergency situation.

Additionally, you can contact Duke Leto via GPG keys from [Keybase](https://keybase.io/dukeleto), corresponding to

    F16219F4C23F91112E9C734A8DFCBF8E5A4D8019
    https://keybase.io/dukeleto/pgp_keys.asc

# What if all the Hush core devs turn evil?

If we are sufficiently hacked, or if we collectively turn evil, the above
resources will not be sufficient to protect you. Luckily, the Hush network is
growing into a larger and more resilient decentralized community everyday.
