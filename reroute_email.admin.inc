<?php

/**
 * @file
 * Reroute Email admin configuration functions.
 */

/**
 * Settings form.
 */
function reroute_email_settings() {
  $config = config('reroute_email.settings');
  $reroute_email_enable = $config->get('enable');
  $reroute_email_address = $config->get('address');
  $reroute_email_enable_message = $config->get('enable_message');
  $reroute_email_enable_dsm = $config->get('enable_dsm');
  $form_override = FALSE;
  $reroute_email_override = settings_get('reroute_email_override');
  if (isset($reroute_email_override) && $reroute_email_override == TRUE) {
    $form_override = TRUE;
    backdrop_set_message(t('Fields have been overridden in your settings.php file.'), 'warning');
    $reroute_email_enable = settings_get('reroute_email_enable', $config->get('enable'));
    $reroute_email_address = settings_get('reroute_email_address', $config->get('address'));
    $reroute_email_enable_message = settings_get('reroute_email_enable_message', $config->get('enable_message'));
    $reroute_email_enable_dsm = settings_get('reroute_email_enable_dsm', $config->get('enable_dsm'));
  }

  // Checkbox to enable or disable email rerouting.
  $form['reroute_email_enable'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable rerouting'),
    '#default_value' => $reroute_email_enable,
    '#disabled'      => $form_override,
    '#description'   => t('Check this box if you want to enable email rerouting. Uncheck to disable rerouting.'),
  );
  $form['reroute_email_address'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Email addresses'),
    '#default_value' => $reroute_email_address,
    '#disabled'      => $form_override,
    '#description'   => t('By default the address is empty, <strong>all outgoing emails will be aborted</strong> and will be recorded in the <a href="@dblog">recent log entries</a>.<br/><br/>Provide a space, comma, or semicolon-delimited list of email addresses to pass through. Every destination email address which is not on this list will be rerouted to the first address on the list.<br/><br/>Example email addresses: server email (if it is set) @sendmail or site email @site_mail.', array('@dblog' => url('admin/reports/dblog'), '@sendmail' => ini_get('sendmail_from'), '@site_mail' => config_get('system.core', 'site_mail'))),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );
  // Checkbox to enable additional information in email body message.
  $form['reroute_email_enable_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show rerouting description in mail body'),
    '#default_value' => $reroute_email_enable_message,
    '#disabled'      => $form_override,
    '#description' => t('Check this box if you would like a message to be inserted into the email body when the mail is being rerouted. Otherwise, SMTP headers will be used to describe the rerouting. If sending rich-text email, leave this unchecked so that the body of the email will not be disturbed.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );
  // Checkbox to enable the display of a Backdrop message after rerouting email.
  $form['reroute_email_enable_dsm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a Backdrop status message after rerouting submitted email'),
    '#default_value' => $reroute_email_enable_dsm,
    '#disabled'      => $form_override,
    '#description' => t('Check this box if you would like a Backdrop status message to be displayed to users after submitting an email to let them know it was rerouted to a different email address.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );

  // Add a submit button
  $form['actions']['#type'] = 'actions';
  $form['actions']['#disabled'] = $form_override;
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validation callback for reroute_email_settings() form.
 */
function reroute_email_settings_validate($form, $form_state) {
  if ($form_state['values']['reroute_email_enable'] == TRUE) {
    // Allow splitting emails by space, comma, semicolon.
    $addresslist = preg_split(REROUTE_EMAIL_EMAIL_SPLIT_RE, $form_state['values']['reroute_email_address'], -1, PREG_SPLIT_NO_EMPTY);
    foreach ($addresslist as $address) {
      if (!valid_email_address($address)) {
        form_set_error('reroute_email_address', t('@address is not a valid email address', array('@address' => $address)));
      }
    }
  }
}

/**
 * Submit handler for reroute_email_settings().
 */
function reroute_email_settings_submit($form, &$form_state) {
  $config = config('reroute_email.settings');
  $config->set('enable', $form_state['values']['reroute_email_enable']);
  $config->set('address', $form_state['values']['reroute_email_address']);
  $config->set('enable_message', $form_state['values']['reroute_email_enable_message']);
  $config->set('enable_dsm', $form_state['values']['reroute_email_enable_dsm']);
  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));
}

/**
 * Form for sending test messages.
 */
function reroute_email_test_email_form() {
  return array(
    'addresses' => array(
      '#type' => 'fieldset',
      '#description' => t('Email addresses are not validated: any valid or invalid email address format could be submitted.'),
      'to' => array(
        '#type' => 'textfield',
        '#title' => t('To'),
        '#required' => TRUE,
      ),
      'cc' => array(
        '#type' => 'textfield',
        '#title' => t('Cc'),
      ),
      'bcc' => array(
        '#type' => 'textfield',
        '#title' => t('Bcc'),
      ),
    ),
    'subject' => array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => t('Reroute Email Test'),
    ),
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Send email'),
    ),
  );
}

/**
 * Submit handler for email test.
 */
function reroute_email_test_email_form_submit(&$form, &$form_state) {
  $to = $form_state['values']['to'];
  $param_keys = array('cc', 'bcc', 'subject', 'body');
  $params = array_intersect_key($form_state['values'], array_flip($param_keys));
  $message = backdrop_mail('reroute_email', 'test_email_form', $to, language_default(), $params);
  if (!empty($message['result'])) {
    backdrop_set_message(t("Test email submitted for delivery."));
  }
}
