<?php
  $num_rows = sizeof($rows);

  $podr = substr($num_rows, -1) == 1 ? "Подобран" : "Подобрано";
  $gifts = substr($num_rows, -1) == 1 ? "подарок" : ((substr($num_rows, -1) == 0 OR substr($num_rows, -1) > 4) ? "подарков" : "подарка");
?>

<h1><?php print("$podr $num_rows $gifts"); ?></h1>

<div id="yashare" class="yashare-auto-init" data-yashareL10n="ru" data-yashareType="none" data-yashareQuickServices="vkontakte,facebook,twitter,odnoklassniki,moimir,lj,gplus"></div>

<div class="row">
    <?php foreach ($rows as $row): ?>
    <div class="col-sm-6 col-md-4">
        <div class="thumbnail">
            <?php print($row['img']); ?>
            <div class="caption">
                <h3><?php print($row['title']); ?></h3>
                <p><?php print($row['body_value']); ?></p>
            </div>
        </div>
    </div>
    <?php endforeach; ?>
</div>