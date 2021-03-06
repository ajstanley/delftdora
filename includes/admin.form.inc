<?php

/**
 * @file
 * Admin callbacks for Islandora Preingest Transform
 */

/**
 * Admin form.
 */
function delft_admin($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $form = array();
  $password = variable_get('delftdora_fedora_password', '');
  $executable_path = variable_get('delftdora_fedora_path', '/usr/local/fedora/server/bin');
  $log_path = variable_get('delftdora_fedora_path', '/usr/local/fedora/server/logs');

  $form['delftdora_fedora_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Password'),
    '#description' => t('Password for user fedoraAdmin.'),
    '#default_value' => $password,
  );

  $form['delftdora_fedora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Executable Path'),
    '#description' => t('Full server Path to fedora-modify script'),
    '#default_value' => $executable_path,
  );
  $form['delftdora_fedora_log_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Logging Path'),
    '#description' => t('Full server path to Fedora logging directory'),
    '#default_value' => $log_path,
  );


  $form['table_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Content Models'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['table_wrapper']['the_table'] = islandora_content_model_select_table_form_element('delft_content_models');
  $form['islandora_transform_submit'] = array(
    '#value' => t('Save Configuration'),
    '#type' => 'submit',
  );
  return $form;
}

/**
 * Admin form validator.
 *
 * @param array $form
 *   incoming form data
 * @param array $form_state
 *   incoming form_state
 */
function delft_admin_validate($form, &$form_state) {
  if (isset($form_state['values'])) {
    $validators = array('file_validate_extensions' => array('xsl', 'xslt'));
    $rels_file = file_save_upload('rels_xslt', $validators);
    if ($rels_file) {
      if ($rels_file = file_move($rels_file, 'public://')) {
        $form_state['storage']['rels_file'] = $rels_file;
      }
      else {
        form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
      }
    }
  }
}

/**
 * Admin form submit.
 *
 * @param array $form
 *   incoming form data
 * @param array $form_state
 *   incoming form_state
 */
function delft_admin_submit($form, &$form_state) {

  if (isset($form_state['storage']['rels_file'])) {
    $file = $form_state['storage']['rels_file'];
    unset($form_state['storage']['rels_file']);
    variable_set("delftdora_rels_transform_xslt", file_get_contents($file->uri));
  }

  $enabled = array_filter($form_state['values']['the_table']);
  variable_set('delftdora_content_models', $enabled);
  variable_set('delftdora_fedora_password', $form_state['values']['delftdora_fedora_password']);
  variable_set('delftdora_fedora_log_path', $form_state['values']['delftdora_fedora_log_path']);
}
