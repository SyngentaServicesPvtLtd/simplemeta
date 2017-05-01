<?php

/**
 * @file
 * Entity API integration, classes, controllers, etc.
 */

/**
 * Class SimplemetaEntity.
 *
 * A common class for SimpleMeta entities.
 */
class SimplemetaEntity extends Entity {

  /**
   * Primary identifier of the SimpleMeta entity.
   *
   * @var int
   */
  public $sid = NULL;

  /**
   * The Drupal path the SimpleMeta entity is related to.
   *
   * @var string
   */
  public $path = NULL;

  /**
   * Saved in the SimpleMeta entity metadata.
   *
   * @var array
   */
  public $data = array();

  /**
   * Language code the SimpleMeta entity is related to.
   *
   * @var string
   */
  public $language = '';

  /**
   * A numeric representation of how specific the path is.
   *
   * @var int
   */
  public $fit = NULL;

  /**
   * {@inheritdoc}
   */
  public function defaultUri() {
    return array(
      'path' => 'admin/content/simplemeta/manage/' . $this->sid,
    );
  }

  /**
   * {@inheritdoc}
   *
   * @todo Build label in another way.
   */
  protected function defaultLabel() {
    return 'SimpleMeta' . (isset($this->sid) ? ' ' . $this->sid : '');
  }

  /**
   * {@inheritdoc}
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $data = $this->data;
    $content = array();

    if ($view_mode == 'meta') {
      foreach ($data as $key => $value) {
        if ($key == 'title') {
          $content[$key] = array(
            '#markup' => check_plain($value),
          );
        }
        $content[$key]['#attached']['drupal_add_html_head'][] = array(
          array(
            '#theme' => 'html_tag',
            '#tag' => 'meta',
            '#attributes' => array(
              'name' => $key,
              'content' => $value,
            ),
          ),
          'simplemeta_' . $key,
        );
      }
    }
    else {
      foreach ($data as $key => $value) {
        $content[$key] = array(
          '#markup' => check_plain($value),
        );
      }
    }

    return entity_get_controller($this->entityType)
      ->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    $this->fit = _simplemeta_meta_calculate_fit($this->path);
    return parent::save();
  }

}

/**
 * Class SimplemetaEntityMetadataController.
 *
 * Controller for generating metadata for SimpleMeta entity type.
 */
class SimplemetaEntityMetadataController extends EntityDefaultMetadataController {

  /**
   * {@inheritdoc}
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['path'] = array(
      'type' => 'text',
      'label' => t('Path'),
      'description' => t('The Drupal path the SimpleMeta entity is related to.'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'path',
    );

    $properties['language'] = array(
      'type' => 'text',
      'label' => t('Source'),
      'description' => t('Language of the SimpleMeta'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'language',
    );

    $properties['fit'] = array(
      'type' => 'integer',
      'label' => t('Fit'),
      'description' => t('A numeric representation of how specific the path is'),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'fit',
    );

    $properties['data'] = array(
      'type' => 'struct',
      'label' => t('Data'),
      'description' => t('Stored metadata'),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'data',
    );

    return $info;
  }

}
