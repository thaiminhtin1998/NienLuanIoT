<div id="page-content-wrapper">
<div class="container-fluid">
<div class="row">
    <div class="col-lg-12">
        <section class="content-header">
            <h1>
                <?php
                    $title = '';
                    $title_sm = '';
                    $query = "SELECT * FROM menus WHERE link='{$menu}' LIMIT 1";
                    $title_result = mysqli_query($con, $query);
                    $menu_temp = mysqli_fetch_assoc($title_result);
                    
                    $title = $menu_temp['page_title'];
                    $title_sm = $menu_temp['page_title_sm'];
                    echo $title . ' <small>'.$title_sm.'</small>';
                ?>
            </h1>
        </section>
    </div>
</div>
<div class="spacer"></div>