Reroute Email
=============
This module intercepts all outgoing emails from a Backdrop CMS site and reroutes them to a predefined configurable email address.

This is useful in case where you do not want email sent from a Backdrop CMS site to reach the users. For example, if you copy a live site to a test site for the purpose of development, and you do not want any email sent to real users of the original site. Or you want to check the emails sent for uniform formatting, footers, ...etc.

Installation
------------
To install this module, do the following:

1. Extract the tar ball that you downloaded.

2. Upload the entire directory and all its contents to your modules directory.

Configuration
-------------
To enable this module do the following:

1. Go to Admin -> Modules, and enable reroute email.

2. Go to Admin -> Configuration -> Development -> Reroute email, and enter an email address to route all email to. If the field is empty and no value is provided for rerouted email addresses, all outgoing emails would be aborted and recorded in the recent log entries, with a full dump of the email variables, which could provide an additional debugging method.

Tips and Tricks
---------------
Reroute Email provides configuration variables that you can directly override in the settings.php file of a site. This is useful for moving sites from live to test and vice versa. To override place the following line in the settings.php file for each environment:

  `$settings['reroute_email_override'] = TRUE;`

To reroute on a test environment you add the following line in the settings.php file:

  `$settings['reroute_email_enable'] = 1;`

And for the live environment, you set it as follows:

  `$settings['reroute_email_enable'] = 0;`

Configuration and all the settings variables can be overridden in the settings.php file by copying and pasting the code snippet below and changing the values:

```
/**
 * Reroute Email module:
 *
 * To override specific variables and ensure that email rerouting is enabled or
 * disabled, change the values below accordingly for your site.
 */
$settings['reroute_email_override'] = TRUE;
// Enable email rerouting.
$settings['reroute_email_enable'] = 1;
// Space, comma, or semicolon-delimited list of email addresses to pass
// through. Every destination email address which is not on this list will be
// rerouted to the first address on the list.
$settings['reroute_email_address'] = "example@example.com";
// Enable inserting a message into the email body when the mail is being
// rerouted.
$settings['reroute_email_enable_message'] = 1;
// Enable the display of a Backdrop CMS status message after rerouting email.
$settings['reroute_email_enable_dsm'] = 1;
```

Test Email Form
---------------
Reroute Email also provides a convenient form for testing email sending or rerouting. After enabling the module, a test email form is accessible under: `Admin -> Configuration -> Development -> Reroute email -> Test email form`

This form allows sending an email upon submission to the recipients entered in the fields To, Cc and Bcc, which is very practical for testing if emails are correctly rerouted to the configured addresses.

License
-------
This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

Maintainers
-----------

Looking for maintainers for Backdrop.

Maintainers of Drupal 7 version:

* Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com) (original author)
* rfay (http://drupal.org/user/30906)
* DYdave (http://drupal.org/user/467284)

Ported and adapted for Backdrop CMS by Herb v/d Dool (https://github.com/herbdool).
