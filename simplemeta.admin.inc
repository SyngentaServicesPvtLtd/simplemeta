<?php

/**
 * @file
 * Admin UI, callbacks and helper functions.
 */

/**
 * SimpleMeta entity form callback.
 *
 * @see entity_form()
 */
function simplemeta_form($form, &$form_state, $meta, $op, $type = NULL) {
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('% may be used as placeholder for system paths, for example, news/archive/%'),
    '#default_value' => $meta->path,
    '#required' => TRUE,
  );
  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => _simplemeta_language_list(),
    '#default_value' => $meta->language,
  );

  $form['data'] = array(
    '#tree' => TRUE,
  );
  // Meta title.
  $form['data']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 255,
    '#default_value' => (isset($meta->data['title'])) ? $meta->data['title'] : '',
  );
  // Meta description.
  $form['data']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => (isset($meta->data['description'])) ? $meta->data['description'] : '',
  );
  $form['data']['keywords'] = array(
    '#type' => 'textfield',
    '#title' => t('Keywords'),
    '#description' => t('Comma-separated list of keywords.'),
    '#maxlength' => 255,
    '#default_value' => (isset($meta->data['keywords'])) ? $meta->data['keywords'] : '',
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 400,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('simplemeta_form_validate_save'),
    '#submit' => array('simplemeta_form_submit_save'),
  );
  if (!empty($meta->sid)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#suffix' => l(t('Cancel'), 'admin/content/simplemeta'),
      '#submit' => array('simplemeta_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#attributes']['class'][] = str_replace('_', '-', __FUNCTION__);
  return $form;
}

/**
 * SimpleMeta entity form validation callback.
 */
function simplemeta_form_validate_save($form, &$form_state) {
  // Do nothing for now.
  // @todo provide some validation or attach validation callbacks directly to form elements
}

/**
 * SimpleMeta entity form submit callback on save op.
 */
function simplemeta_form_submit_save($form, &$form_state) {
  $entity = entity_ui_controller('simplemeta')->entityFormSubmitBuildEntity($form, $form_state);
  $entity->save();
  $form_state['redirect'] = 'admin/content/simplemeta';
  drupal_set_message(t('SimpleMeta has been saved.'));
}

/**
 * SimpleMeta entity form submit callback on delete op.
 */
function simplemeta_form_submit_delete($form, &$form_state) {
  $entity = $form_state['simplemeta'];
  $form_state['redirect'] = $form_state['redirect'] = 'admin/content/simplemeta/manage/' . $entity->sid . '/delete';
}

/**
 * SimpleMeta settings form.
 */
function simplemeta_settings_form($form, &$form_state) {
  $form['simplemeta_form_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Add Meta Tags Form'),
    '#description' => t('If enabled, form will appear on pages'),
    '#default_value' => variable_get('simplemeta_form_enable', TRUE),
    '#return_value' => TRUE,
  );
  $form['simplemeta_language_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Language Support'),
    '#description' => t('If enabled, language selectbox will appear on the form. Otherwise all created metadata will be language-neutral i.e. global for all languages'),
    '#default_value' => variable_get('simplemeta_language_enable', FALSE),
    '#return_value' => TRUE,
  );
  return system_settings_form($form);
}
