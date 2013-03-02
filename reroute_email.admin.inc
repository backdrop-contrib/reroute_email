<?php

/**
 * @file
 * Reroute Email admin configuration functions.
 */

/**
 * Settings form.
 */
function reroute_email_settings() {
  $form[REROUTE_EMAIL_ENABLE] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable rerouting'),
    '#default_value' => variable_get(REROUTE_EMAIL_ENABLE, 0),
    '#description'   => t('Check this box if you want to enable email rerouting. Uncheck to disable rerouting.'),
  );
  $form[REROUTE_EMAIL_ADDRESS] = array(
    '#type'          => 'textfield',
    '#title'         => t('Email addresses'),
    '#default_value' => variable_get(REROUTE_EMAIL_ADDRESS, ini_get('sendmail_from')),
    '#description'   => t('Provide a space, comma, or semicolon-delimited list of email addresses to pass through. Every destination email address which is not on this list will be rerouted to the first address on the list.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );
  $form[REROUTE_EMAIL_ENABLE_MESSAGE] = array(
    '#type' => 'checkbox',
    '#title' => t('Show rerouting description in mail body'),
    '#default_value' => variable_get(REROUTE_EMAIL_ENABLE_MESSAGE, 1),
    '#description' => t('Check this box if you want a message to be inserted into the email body when the mail is being rerouted. Otherwise, SMTP headers will be used to describe the rerouting. If sending rich-text email, leave this unchecked so that the body of the email will not be disturbed.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );

  return system_settings_form($form);
}

/**
 * Validation callback for reroute_email_settings() form.
 */
function reroute_email_settings_validate($form, $form_state) {
  if ($form_state['values']['reroute_email_enable'] == TRUE) {
    // Allow splitting emails by space, comma, semicolon.
    $addresslist = preg_split(EMAIL_SPLIT_RE, $form_state['values']['reroute_email_address'], -1, PREG_SPLIT_NO_EMPTY);
    foreach ($addresslist as $address) {
      if (!valid_email_address($address)) {
        form_set_error('reroute_email_address', t('@address is not a valid email address', array('@address' => $address)));
      }
    }
  }
}
