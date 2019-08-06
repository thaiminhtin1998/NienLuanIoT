<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
    <div class="container-fluid">
        <!-- Brand and toggle get grouped for better mobile display -->
        <div class="navbar-header">
            <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand pull-left " href="#menu-toggle" id="menu-toggle"><span class="glyphicon glyphicon-chevron-left soft-white"></span></a>
            <a class="navbar-brand pull-left" id='space' href="#">STAY ONLINE</a>
        </div>
        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav navbar-right">
            <!-- function output modules list -->
            <?php
                if($con){
                    $query = "SELECT * FROM modules ORDER BY seq;";
                    $result = mysqli_query($con, $query);
                    while($row=mysqli_fetch_assoc($result)){
                        $query = "SELECT * FROM menus WHERE parent_id={$row['id']} ORDER BY seq LIMIT 1;";
                        $menu_result = mysqli_query($con, $query);
                        $menu_temp = mysqli_fetch_assoc($menu_result);
                        $menu_temp = $menu_temp['link'];
                        
                        echo '<li '; if($module==$row['link']) echo 'class="active"'; echo '>';
                        echo '<a href="../' . $row['link'] .'/index.php?mod='.$row['link'].'&menu='. $menu_temp . '" title="'.$row['description'].'">'.$row['name'].'</a>';
                        echo '</li>';
                    }
                }
            ?>
            <!-- END -->
            <li class="dropdown <?php if($module == "base") echo "active"; ?>">
              <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false"><span class="glyphicon glyphicon-user"></span>&nbsp; <?php echo $_SESSION['username']; ?> <span class="caret"></span></a>
              <ul class="dropdown-menu">
                <?php
                    if($_SESSION['username'] != 'admin'){
                        echo '<li><a href="../config/profile.php?mod=base&user=' . $_SESSION["user_id"].'">Profile</a></li>';
                 }
                ?>
                <?php
                    if($_SESSION['username'] == 'admin'){
                        echo '<li><a href="../config/index.php?mod=base&menu=db">Configurations</a></li>';
                        echo '<li><a href="../config/index.php?mod=base&menu=log">Logs</a></li>';
                        echo '<li><a href="../config/index.php?mod=base&menu=doc">Help</a></li>';
                        
                    }
                ?>
                <li role="separator" class="divider"></li>
                <li><a href="../base/includes/logout.php">Logout</a></li>
              </ul>
            </li>
          </ul>
        </div>
        <!-- /.navbar-collapse -->
    </div>
    <!-- /.container -->
</nav>