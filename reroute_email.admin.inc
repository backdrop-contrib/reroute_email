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
    $addresslist = preg_split(REROUTE_EMAIL_EMAIL_SPLIT_RE, $form_state['values']['reroute_email_address'], -1, PREG_SPLIT_NO_EMPTY);
    foreach ($addresslist as $address) {
      if (!valid_email_address($address)) {
        form_set_error('reroute_email_address', t('@address is not a valid email address', array('@address' => $address)));
      }
    }
  }
}

/**
 * Form for sending test messages.
 */
function reroute_email_test_email_form() {
  return array(
    'addresses' => array(
      '#type' => 'fieldset',
      'to' => array(
        '#type' => 'textfield',
        '#title' => t('To'),
        '#required' => TRUE,
        '#element_validate' => array('reroute_email_test_email_field_validate'),
      ),
      'cc' => array(
        '#type' => 'textfield',
        '#title' => t('Cc'),
        '#element_validate' => array('reroute_email_test_email_field_validate'),
      ),
      'bcc' => array(
        '#type' => 'textfield',
        '#title' => t('Bcc'),
        '#element_validate' => array('reroute_email_test_email_field_validate'),
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
  $message = drupal_mail('reroute_email', 'test_email_form', $to, language_default(), $params);
  if (!empty($message['result'])) {
    drupal_set_message(t("Test email submitted for delivery."));
  }
}

/**
 * Email fields validation callback for email address format.
 */
function reroute_email_test_email_field_validate($element, &$form_state, $form) {
  if (!empty($element['#value']) && !valid_email_address($element['#value'])) {
    form_error($element, t('Please submit a valid email address for the field @field_label.', array('@field_label' => $element['#title'])));
  }
}
