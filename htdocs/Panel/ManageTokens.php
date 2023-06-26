<?php
include 'includes/database.php';
include 'includes/settings.php';




$tokens = mysqli_query($con, "SELECT COUNT(1) FROM `tokens`");
$tokens_row = mysqli_fetch_array($tokens);
$tokens_total = $tokens_row[0];
$today = time("Y-m-d");


?>

<!DOCTYPE html>
<html>
    
<head>
        
        <!-- Title -->
        <title>OutHereLive| Admin Dashboard</title>
        <meta content="width=device-width, initial-scale=1" name="viewport"/>
        <meta charset="UTF-8">
        <meta name="description" content="OutHereLiveBackstage" />
        <meta name="keywords" content="admin,Backstage,OutHereLive,Backend,Emodz" />
        <meta name="author" content="Emodz" />
        
        <!-- Styles -->
        <link href='http://fonts.googleapis.com/css?family=Ubuntu:300,400,500,700' rel='stylesheet' type='text/css'>
        <link href="assets/plugins/pace-master/themes/blue/pace-theme-flash.css" rel="stylesheet"/>
        <link href="assets/plugins/uniform/css/uniform.default.min.css" rel="stylesheet"/>
        <link href="assets/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/fontawesome/css/font-awesome.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/line-icons/simple-line-icons.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/waves/waves.min.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/switchery/switchery.min.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/3d-bold-navigation/css/style.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/slidepushmenus/css/component.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/weather-icons-master/css/weather-icons.min.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/metrojs/MetroJs.min.css" rel="stylesheet" type="text/css"/>
        	
        <!-- Theme Styles -->
        <link href="assets/css/modern.min.css" rel="stylesheet" type="text/css"/>
        <link href="assets/css/custom.css" rel="stylesheet" type="text/css"/>
        
        <script src="assets/plugins/3d-bold-navigation/js/modernizr.js"></script>
        
		<main class="page-content content-wrap">
            <div class="navbar">
                <div class="navbar-inner container">
                    <div class="sidebar-pusher">
                    </div>
                    <div class="logo-box">
                        <a href="index.html" class="logo-text"><span><font color=981E8E>OutHereLive</font></span></a>
                    </div><!-- Logo Box -->
					<ul class="nav navbar-nav navbar-right">
						<li> <a href="http://logout@www.OutHerelive.tk"><font color=981E8E>Sign out</font> </a> </li>
					</ul><!-- Nav -->
				</div><!-- Top Menu -->
            </div><!-- Navbar -->
            <div class="page-sidebar sidebar horizontal-bar">
                <div class="page-sidebar-inner">
                    <ul class="menu accordion-menu">
                        <li class="nav-heading"><span>Navigation</span></li>
                        <li><a href="Home.php"><span class="menu-icon icon-speedometer"></span><p>Home</p></a></li>
                        <li class="droplink"><a href="ManageClients.php"><span class="menu-icon icon-user"></span><p>Manage Clients</p></a></li>
                        <li class="active"><a href="ManageTokens.php"><span class="menu-icon icon-tag"></span><p>Manage Tokens</p><span class="arrow"></span></a></li>
						<li class="droplink"><a href="FailedClients.php"><span class="menu-icon icon-close"></span><p>Failed Clients</p><span class="arrow"></span></a></li>
                        <li class="droplink"><a href="/OutHere.zip"><span class="menu-icon icon-present"></span><p>Download Files</p><span class="arrow"></span></a></li>
                    </ul>
				</div>
			</div>
			<?php
			if(isset($_POST['redeem']))
			{
				$redeem_token = $_POST['tokenVal'];
				$redeem_name = $_POST['nameVal'];
				$redeem_cpuk = $_POST['cpukeyVal'];
				$redeem_email = $_POST['emailVal'];

				$check_token = mysqli_query($con, "SELECT * FROM `tokens` WHERE `token` = '".$redeem_token."'");
				$num_token = mysqli_num_rows($check_token);
				$row = mysqli_fetch_array($check_token);
				$used = $row['used'];
				$client_time = $row['time'];
				$time_now = new datetime(date("Y-m-d"));
				$time_now->add(new Dateinterval('P'.$client_time.'D'));
				$time = $time_now->format("Y-m-d") . "\n";
				if($num_token == 1)
				{
					if($used == 0)
					{
						$insert = mysqli_query($con, "INSERT INTO `consoles`(`id`, `name`, `cpukey`, `email`, `time`, `enabled`) VALUES ('NULL', '".$redeem_name."', '".$redeem_cpuk."', '".$redeem_email."','".$time."','1')");
						$token_off = mysqli_query($con, "UPDATE `tokens` SET `used` = '1' WHERE `token` = '".$redeem_token."'");
						echo '<div class="row"><div class="col-md-12"><div class="alert alert-success">
						<button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
						<strong>OK!</strong> Client successfully added with token.
						</div></div></div>';
					}
					elseif($used == 1)
					{
						echo '<div class="row"><div class="col-md-12"><div class="alert alert-success">
						<button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
						<strong>Fail!</strong> This token has already been used, try another token!.
						</div></div></div>';
					}
				}
				elseif($num_token == 0)
				{
					echo '<div class="row"><div class="col-md-12"><div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
					<strong>Fail!</strong> This token does not exist in our database.
					</div></div></div>';
				}
				elseif($num_token > 1)
				{
					echo '<div class="row"><div class="col-md-12"><div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
					<strong>Fail!</strong> This token has been duplicated!.
					</div></div></div>';
				}
			}
			?>
		
		
			<?php
			function randString($length, $charset='ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789')
			{
				$str = '';
				$count = strlen($charset);
				while ($length--) {
					$str .= $charset[mt_rand(0, $count-1)];
				}
				return $str;
			}
			?>
           
            <?php
            if(isset($_POST['create']))
            {
              $_time = $_POST['days'];
              $token_semi_quantity = 0;
              $token_combo = $_POST['howmany'];
              while($token_semi_quantity < $token_combo)
              {
				  $token = randString(4) . "-" . randString(4) . "-" . randString(4);
               // $token = openssl_digest(uniqid(rand(),true), "sha1");
                $token_semi_quantity++;
				$token_time = $_POST['days'];
                $insert_token = mysqli_query($con, "INSERT INTO `tokens`(`id`, `token`, `time`, `generatedby`, `timegenerated`, `used`) VALUES ('NULL', '".$token."','".$token_time."', '".$_SERVER['PHP_AUTH_USER']."', '".date("Y-m-d")."', '0')");
              }
            }
            ?>

		<br>
		<div class="container-flid" style="width:100%">
		<div class="col-md-4 col-md-offset-4">
		<div class="row">
		<div class="col-xs-16">
		<div class="panel panel-success">
		<form method="POST">
		<div class="panel-heading">
			<h3 class="panel-title">OutHereLive- Generate Token System</h3>
		</div>
		<div class="panel-body controls no-padding">
		<div class="row-form">
		<br>
		<div class="col-md-3"><strong>How Many:</strong></div>
		<div class="col-md-9">
		<select class="form-control" name="howmany">
			<option value="1">1</option>
			<option value="2">2</option>
			<option value="3">3</option>
			<option value="4">4</option>
			<option value="5">5</option>
			<option value="6">6</option>
			<option value="7">7</option>
			<option value="8">8</option>
			<option value="9">9</option>
			<option value="10">10</option>
			<option value="11">11</option>
			<option value="12">12</option>
			<option value="13">13</option>
			<option value="14">14</option>
			<option value="15">15</option>
			<option value="16">16</option>
			<option value="17">17</option>
			<option value="18">18</option>
			<option value="19">19</option>
			<option value="20">20</option>
			<option value="21">21</option>
			<option value="22">22</option>
			<option value="23">23</option>
			<option value="24">24</option>
			<option value="25">25</option>
			<option value="26">26</option>
			<option value="27">27</option>
			<option value="28">28</option>
			<option value="29">29</option>
			<option value="30">30</option>
		</select>
		</div>
		</div>
		<div class="row-form">
		  <div class="col-md-3"><strong>Days:</strong></div>
		  <div class="col-md-9">
			<select class="form-control" name="days">
			  <option value="1">1 Day</option>
			  <option value="3">3 Days</option>
			  <option value="7">7 Days</option>
			  <option value="14">14 Days</option>
			  <option value="31">1 Month</option>
			  <option value="93">3 Month</option>
			  <option value="186">6 month</option>
			  <option value="365">1 year</option>
			  <option value="99999">Lifetime</option>
			</select>
		  </div>
		</div>
		</div>
		<center><div class="panel-footer"><button class="btn btn-success" name="create">Generate Token</button></div></center>
		</form>
		</div>
		</div>
		</div>
		</div>
		</div>
		
		
		<br></br><br></br>
		
			<div class="container-flid" style="width:100%">
			<div class="col-md-10 col-md-offset-2">
			<div class="row">
			<div class="col-xs-10">
			<div class="panel panel-success">
			<div class="panel-heading">
			  <h3 class="panel-title">OutHereLive- Tokens (Total: <?php echo $tokens_total; ?>)</h3>
			</div>
			<div class="panel-body controls no-padding">
			<div class="block">
				<table cellpadding="0" cellspacing="0" width="100%" class="table table-bordered table-striped sortable">
				  <thead>
					<tr>
					  <th>Token</th>
					  <th>Durability</th>
					  <th>Generated By</th>
					  <th>Time Generated</th>
					  <th>Used By</th>
					  <th>Used</th>					 
					  <th>Delete</th>
					</tr>
					</thead>
					<tbody>
					<?php

					$tokens_result = mysqli_query($con, "SELECT * FROM `tokens`");
					while($tokens_rows2 = mysqli_fetch_array($tokens_result))
					{
						echo'
						<tr>
						<td>
						'.$tokens_rows2['token'].'
						</td>
						<td>
						'.($tokens_rows2['time'] == "99999" ? "LIFETIME" : $tokens_rows2['time'].' Days').'
						</td>
						<td>
						'.$tokens_rows2['generatedby'].'
						</td>
						<td>
						'.$tokens_rows2['timegenerated'].'
						</td>
						<td>
						'.$tokens_rows2['usedby'].'
						</td>
						<td>
						<center>'.($tokens_rows2['used'] == "0" ? "<label class = 'label label-primary'>NOT USED</label>" : "<label class = 'label label-success'>USED</label>").'</center>
						</td>
						<td>
						<form action = "ManageTokens.php?id='.$tokens_rows2['id'].'" method="post">
						<center><button type="submit" name = "deletetoken" class="btn btn-success"><i class="fa fa-cog"></i> Delete Token</button></center>
						</form>
						</td>
						'
						;
if(isset($_GET['id']))
{
	$id = $_GET['id'];
	$deletetoken = mysqli_query($con, "DELETE FROM `tokens` WHERE `id` = '".$_GET['id']."'");

		echo '
  <script language ="javascript">
  window.location.href = "ManageTokens.php"
  </script>
  ';
	}

					
					}

					?>
					</tr>
					</tbody>
				</table>
			</div>
			</div>
			</div>
			</div>
			</div>
			</div>
			</div>
			
			<br><br><br><br><br><br><br><br><br><br><br><br><br><br>
								
			</div><!-- Main Wrapper -->
			<div class="page-footer">
				<div class="container">
					<center><p class="no-s">2016 &copy; OutHereLiveby Emodz.</p></center>
				</div>
			</div>
 
        <div class="cd-overlay"></div>
	
        <!-- Javascripts -->
        <script src="assets/plugins/jquery/jquery-2.1.4.min.js"></script>
        <script src="assets/plugins/jquery-ui/jquery-ui.min.js"></script>
        <script src="assets/plugins/pace-master/pace.min.js"></script>
        <script src="assets/plugins/jquery-blockui/jquery.blockui.js"></script>
        <script src="assets/plugins/bootstrap/js/bootstrap.min.js"></script>
        <script src="assets/plugins/jquery-slimscroll/jquery.slimscroll.min.js"></script>
        <script src="assets/plugins/switchery/switchery.min.js"></script>
        <script src="assets/plugins/uniform/jquery.uniform.min.js"></script>
        <script src="assets/plugins/classie/classie.js"></script>
        <script src="assets/plugins/waves/waves.min.js"></script>
        <script src="assets/plugins/3d-bold-navigation/js/main.js"></script>
        <script src="assets/plugins/waypoints/jquery.waypoints.min.js"></script>
        <script src="assets/plugins/jquery-counterup/jquery.counterup.min.js"></script>
        <script src="assets/plugins/toastr/toastr.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.time.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.symbol.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.resize.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.tooltip.min.js"></script>
        <script src="assets/plugins/curvedlines/curvedLines.js"></script>
        <script src="assets/plugins/metrojs/MetroJs.min.js"></script>
    </body>
</html>