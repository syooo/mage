<?php




function mage_get_gifts_all()
{
  $q = db_select('node', 'node');

  $q->fields('node', array('nid', 'title'));
  $q->fields('body', array('body_value'));
  $q->fields('img', array('filename'));
  //$q->fields('link', array('field_link_value'));

  $q->orderBy('created', 'DESC');


  $q->leftjoin("field_data_body", "body", "node.nid = body.entity_id");
  //$q->leftjoin("field_data_field_link", "link", "node.nid = link.entity_id");
  $q->leftjoin("field_data_field_picture", "picture", "node.nid = picture.entity_id");
  $q->leftjoin("file_managed", "img", "picture.field_picture_fid = img.fid");

  return $q->execute();
}

function mage_get_gifts($val)
{
  $q = db_select('node', 'node');

  $q->fields('node', array('nid', 'title'));
  $q->fields('body', array('body_value'));
  $q->fields('img', array('filename'));
  //$q->fields('link', array('field_link_value'));

  $q->leftjoin("field_data_body", "body", "node.nid = body.entity_id");
  //$q->leftjoin("field_data_field_link", "link", "node.nid = link.entity_id");
  $q->leftjoin("field_data_field_picture", "picture", "node.nid = picture.entity_id");
  $q->leftjoin("file_managed", "img", "picture.field_picture_fid = img.fid");

  $q->orderBy('created', 'DESC');

  // Пол
  if (isset($val['field_sex']))
  {
    $sex[] = $val['field_sex'] == 1 ? 0 : 1; 
    $q->leftjoin("field_data_field_sex", "sex", "node.nid = sex.entity_id");
    $q->condition('sex.field_sex_value', $sex, 'NOT IN');    
  }

  // Возраст
  if (isset($val['field_age_min']) && isset($val['field_age_max']))
  {
    $q->leftjoin("field_data_field_age_min", "age_min", "node.nid = age_min.entity_id");
    $q->leftjoin("field_data_field_age_max", "age_max", "node.nid = age_max.entity_id");

    $q->condition('age_min.field_age_min_value', $val['field_age_min'],'>=');  
    $q->condition('age_max.field_age_max_value', $val['field_age_max'],'<='); 
  }

  // Цена
  if (isset($val['field_age_min']) && isset($val['field_age_max']))
  {
    $q->leftjoin("field_data_field_price_min", "price_min", "node.nid = price_min.entity_id");
    $q->leftjoin("field_data_field_price_max", "price_max", "node.nid = price_max.entity_id");

    $price_min = (int)str_replace(".", "", $val['field_price_min']);
    $price_max = (int)str_replace(".", "", $val['field_price_max']);

    $q->condition('price_min.field_price_min_value', $price_min,'>=');  
    $q->condition('price_max.field_price_max_value', $price_max,'<='); 
  }

  // Семейное положение
  if (isset($val['field_relationship']))
  {
    $q->leftjoin("field_data_field_relationship", "relationship", "node.nid = relationship.entity_id");
    $q->condition('relationship.field_relationship_value', $val['field_relationship'],'=');   
  }

  // Сфера деятельности
  if (isset($val['field_activity'])) 
  {
    foreach ($val['field_activity'] as $key => $value) {
      if (!$value)
        unset($val['field_activity'][$key]);
    }

    if (sizeof($val['field_activity']) > 0)
    {
      $q->leftjoin("field_data_field_activity", "activity", "node.nid = activity.entity_id");
      $q->condition('activity.field_activity_value ', array_values($val['field_activity']),'IN');
    }
  }

  // Знак зодиака
  if (isset($val['field_astrological_sign']))
  {
    $q->leftjoin("field_data_field_astrological_sign", "astrological_sign", "node.nid = astrological_sign.entity_id");
    $q->condition('astrological_sign.field_astrological_sign_value', $val['field_astrological_sign'],'=');    
  }

  // Интересы
  if (isset($val['field_interests'])) 
  {
    foreach ($val['field_interests'] as $key => $value) {
      if (!$value)
        unset($val['field_interests'][$key]);
    }

    if (sizeof($val['field_interests']) > 0)
    {
      $q->leftjoin("field_data_field_interests", "interests", "node.nid = interests.entity_id");
      $q->condition('interests.field_interests_value ', array_values($val['field_interests']),'IN');
    }
  }

  // Повод
  if (isset($val['field_occasion'])) 
  {
    foreach ($val['field_occasion'] as $key => $value) {
      if (!$value)
        unset($val['field_occasion'][$key]);
    }

    if (sizeof($val['field_occasion']) > 0)
    {
      $q->leftjoin("field_data_field_occasion", "occasion", "node.nid = occasion.entity_id");
      $q->condition('occasion.field_occasion_value ', array_values($val['field_occasion']),'IN');
    }
  }

  //$query->distinct()

  return $q->execute();
}

function mage_get_form_main()
{
  $fields = field_info_fields();

  $form['sex'] = array(
    '#type' => 'fieldset', 
  );

  $form['sex']['sex_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Выберите пол',
    '#suffix' => '</h3>'
  );

  $form['sex']['field_sex'] = array(
    '#type' => 'radios',
    '#options' => list_allowed_values($fields["field_sex"]),
  );

  $form['sex']['next_sex'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-age" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );



  $form['age'] = array(
    '#type' => 'fieldset', 
  );

  $form['age']['age_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Возраст?',
    '#suffix' => '</h3>'
  );

  $form['age']['field_age_min'] = array(
    '#type' => 'textfield',
    '#default_value' => 1,
    '#size' => 3
  );

  $form['age']['field_age_max'] = array(
    '#type' => 'textfield',
    '#default_value' => 115,
    '#size' => 3
  );

  $form['age']['slider_age'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="slider-age"></div>'
  );

  $form['age']['next_age'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-price" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );



  $form['price'] = array(
    '#type' => 'fieldset', 
  );

  $form['price']['price_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'В какую цену?',
    '#suffix' => '</h3>'
  );

  $form['price']['field_price_min'] = array(
    '#type' => 'textfield',
    '#default_value' => "100 р",
    '#size' => 12
  );

  $form['price']['field_price_max'] = array(
    '#type' => 'textfield',
    '#default_value' => "300.000.000€",
    '#size' => 12
  );

  $form['price']['slider_price'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="slider-price"></div>'
  );

  $form['price']['next_price'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-relationship" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );



  $form['relationship'] = array(
    '#type' => 'fieldset', 
  );

  $form['relationship']['relationship_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Семейное положение?',
    '#suffix' => '</h3>'
  );

  $form['relationship']['field_relationship'] = array(
    '#type' => 'radios',
    '#options' => list_allowed_values($fields["field_relationship"]),
  );

  $form['relationship']['slider_relationship'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="slider-relationship"></div>'
  );

  $form['relationship']['next_relationship'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-activity" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );


  $form['activity'] = array(
    '#type' => 'fieldset', 
  );

  $form['activity']['activity_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Сфера деятельности?',
    '#suffix' => '</h3>'
  );

  $form['activity']['field_activity'] = array(
    '#type' => 'checkboxes',
    '#options' => list_allowed_values($fields["field_activity"]),
  );

  $form['activity']['next_activity'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-astrological-sign" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );


  $form['astrological_sign'] = array(
    '#type' => 'fieldset', 
  );

  $form['astrological_sign']['astrological_sign_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Знак зодиака?',
    '#suffix' => '</h3>'
  );

  $form['astrological_sign']['field_astrological_sign'] = array(
    '#type' => 'radios',
    '#options' => list_allowed_values($fields["field_astrological_sign"]),
  );

  $form['astrological_sign']['next_astrological_sign'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-interests" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );




  $form['interests'] = array(
    '#type' => 'fieldset', 
  );

  $form['interests']['interests_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Интересы?',
    '#suffix' => '</h3>'
  );

  $form['interests']['field_interests'] = array(
    '#type' => 'checkboxes',
    '#options' => list_allowed_values($fields["field_interests"]),
  );

  $form['interests']['next_interests'] = array(
    '#type' => 'markup',
    '#prefix' => '<a href="#edit-occasion" class="anchor">',
    '#markup' => t('Ok'),
    '#suffix' => '</a>'
  );



  $form['occasion'] = array(
    '#type' => 'fieldset', 
  );

  $form['occasion']['occasion_h'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>',
    '#markup' => 'Повод?',
    '#suffix' => '</h3>'
  );

  $form['occasion']['field_occasion'] = array(
    '#type' => 'checkboxes',
    '#options' => list_allowed_values($fields["field_occasion"]),
  );

  $form['occasion']['submit'] = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => 'mage_submit_driven_callback',
      /*'wrapper' => 'result',*/
    ),
    '#value' => t('Ok'),
  );

    return $form;
}