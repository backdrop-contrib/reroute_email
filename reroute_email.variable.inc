<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_variable_info().
 */
function reroute_email_variable_info() {
  $variable = array();
  $config = config('reroute_email.settings');
  // Set the email address or use default if not present
  $reroute_email_address = ini_get('sendmail_from');
  if (!empty(config_get('system.core', 'site_mail'))) $reroute_email_address = config_get('system.core', 'site_mail');
  if (!empty($config->get('address'))) $reroute_email_address =  $reroute_email_address = $config->get('address');

  $variable['reroute_email_enable'] = array(
    'title' => t('Enable rerouting'),
    'description' => t('Check this box if you want to enable email rerouting. Uncheck to disable rerouting.'),
    'type' => 'boolean',
    'default' => 0,
    'group' => 'reroute_email',
  );

  $variable['reroute_email_address'] = array(
    'title' => t('Email addresses'),
    'description' => t('Provide a space, comma, or semicolon-delimited list of email addresses to pass through. Every destination email address which is not on this list will be rerouted to the first address on the list.'),
    'default' => $reroute_email_address,
    'group' => 'reroute_email',
  );

  $variable['reroute_email_enable_message'] = array(
    'title' => t('Show rerouting description in mail body'),
    'description' => t('Check this box if you want a message to be inserted into the email body when the mail is being rerouted. Otherwise, SMTP headers will be used to describe the rerouting. If sending rich-text email, leave this unchecked so that the body of the email will not be disturbed.'),
    'type' => 'boolean',
    'default' => 1,
    'group' => 'reroute_email',
  );

  // Checkbox to enable the display of a Backdrop message after rerouting email.
  $variable['reroute_email_enable_dsm'] = array(
    'title' => t('Display a Backdrop status message after rerouting submitted email'),
    'description' => t('Check this box if you would like a Backdrop status message to be displayed to users after submitting an email to let them know it was rerouted to a different email address.'),
    'type' => 'boolean',
    'default' => 1,
    'group' => 'reroute_email',
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function reroute_email_variable_group_info() {
  $groups['reroute_email'] = array(
    'title' => t('Reroute Email'),
    'description' => t('Reroute email configuration settings'),
    'access' => 'administer reroute email',
    'path' => array('admin/config/development/reroute_email'),
  );
  return $groups;
}
