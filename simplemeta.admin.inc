<?php

/**
 * Simplemeta settings form
 */
//function simplemeta_settings_form(&$form_state) {
//  $form = array();
//  $form['simplemeta_form_enable'] = array(
//    '#type' => 'checkbox',
//    '#title' => t('Enable Add Meta Tags Form'),
//    '#description' => t('If enabled, form will appear on pages'),
//    '#default_value' => variable_get('simplemeta_form_enable', FALSE),
//    '#return_value' => TRUE,
//  );
//  return system_settings_form($form);
//}

function simplemeta_meta_list() {
  $info = simplemeta_get_info();
  $query = "SELECT s.* FROM {simplemeta} s ORDER BY s.sid DESC";
  $count_query = "SELECT COUNT(s.sid) FROM {simplemeta} s";
  $pager_per_page = 20;
  $pager_element = 0;
  $result = pager_query($query, $pager_per_page, $pager_element, $count_query);
  
  $items = array();
  while ($meta = db_fetch_object($result)) {
    $meta->data = unserialize($meta->data);
    $items[] = $meta;
  }
  
  return theme('simplemeta_meta_list', $items) . theme_pager(array(), $pager_per_page, $pager_element);
}

function simplemeta_add() {
  $meta = new stdClass();
  $meta->data = array();
  $meta->language = '';
  return drupal_get_form('simplemeta_meta_form', $meta);
}

function simplemeta_meta_delete_confirm(&$form_state, $meta) {
  $form['sid'] = array(
    '#type' => 'value', 
    '#value' => $meta->sid,
  );

  return confirm_form($form, 
    t('Are you sure you want to delete Simplemeta %sid?', array('%sid' => $meta->sid)), 
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/simplemeta/' . $meta->sid . '/edit', 
    t('This action cannot be undone.'), 
    t('Delete'), 
    t('Cancel')
  );
}

function simplemeta_meta_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    simplemeta_meta_delete($form_state['values']['sid']);
    cache_clear_all('*', 'cache_simplemeta', TRUE);
  }
  $form_state['redirect'] = 'admin/content/simplemeta/list';
}

