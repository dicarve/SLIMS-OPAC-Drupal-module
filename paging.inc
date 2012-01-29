<?php
/**
 * simbio_paging_ajax
 * Paging Generator class
 *
 * Copyright (C) 2007,2008  Arie Nugraha (dicarve@yahoo.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

function paging($int_all_recs_num, $int_recs_each_page, $int_pages_each_set = 10, $str_fragment = '', $str_target_frame = '_self') {
    // check for wrong arguments
    if ($int_recs_each_page > $int_all_recs_num) {
        return;
    }

    // total number of pages
    $_num_page_total = ceil($int_all_recs_num/$int_recs_each_page);

    if ($_num_page_total < 2) {
        return;
    }

    // total number of pager set
    $_pager_set_num = ceil($_num_page_total/$int_pages_each_set);

    // check the current page number
    if (isset($_GET['page']) AND $_GET['page'] > 1) {
        $_page = (integer)$_GET['page'];
    } else {$_page = 1;}

    // check the query string
    if (isset($_SERVER['QUERY_STRING']) AND !empty($_SERVER['QUERY_STRING'])) {
        parse_str($_SERVER['QUERY_STRING'], $arr_query_var);
        // rebuild query str without "page" var
        $_query_str_page = '';
        foreach ($arr_query_var as $varname => $varvalue) {
            if (is_string($varvalue)) {
                $varvalue = urlencode($varvalue);
                if ($varname != 'page') {
                    $_query_str_page .= $varname.'='.$varvalue.'&';
                }
            } else if (is_array($varvalue)) {
                foreach ($varvalue as $e_val) {
                    if ($varname != 'page') {
                        $_query_str_page .= $varname.'[]='.$e_val.'&';
                    }
                }
            }
        }
        // append "page" var at the end
        $_query_str_page .= 'page=';
        // create full URL
        $_current_page = $_SERVER['PHP_SELF'].'?'.$_query_str_page;
    } else {
        $_current_page = $_SERVER['PHP_SELF'].'?page=';
    }

    // target frame
    $str_target_frame = 'target="'.$str_target_frame.'"';

    // init the return string
    $_buffer = '<div class="pager">';
    $_stopper = 1;

    // count the offset of paging
    if (($_page > 5) AND ($_page%5 == 1)) {
        $_lowest = $_page-5;
        if ($_page == $_lowest) {
            $_pager_offset = $_lowest;
        } else {
            $_pager_offset = $_page;
        }
    } else if (($_page > 5) AND (($_page*2)%5 == 0)) {
        $_lowest = $_page-5;
        $_pager_offset = $_lowest+1;
    } else if (($_page > 5) AND ($_page%5 > 1)) {
        $_rest = $_page%5;
        $_pager_offset = $_page-($_rest-1);
    } else {
        $_pager_offset = 1;
    }

    // Previous page link
	$_first = t('First Page');
	$_prev = t('Previous');

    if ($_page > 1) {
        $_buffer .= ' &nbsp;';
        $_buffer .= '<a class="pager-item pager-first" href="'.$_current_page.(1).$str_fragment.'" '.$str_target_frame.'>'.$_first.'</a>&nbsp; '."\n";
        $_buffer .= ' &nbsp;';
        $_buffer .= '<a class="pager-item pager-previous" href="'.$_current_page.($_page-1).$str_fragment.'" '.$str_target_frame.'>'.$_prev.'</a>&nbsp; '."\n";
    }

    for ($p = $_pager_offset; ($p <= $_num_page_total) AND ($_stopper < $int_pages_each_set+1); $p++) {
        if ($p == $_page) {
            $_buffer .= ' &nbsp;<strong class="pager-item pager-current">'.$p.'</strong>&nbsp; '."\n";
        } else {
            $_buffer .= ' &nbsp;';
            $_buffer .= '<a class="pager-item" href="'.$_current_page.$p.$str_fragment.'" '.$str_target_frame.'>'.$p.'</a>&nbsp; '."\n";
        }

        $_stopper++;
    }

    // Next page link
	$_next = t('Next');

    if (($_pager_offset != $_num_page_total-4) AND ($_page != $_num_page_total)) {
        $_buffer .= ' &nbsp;';
        $_buffer .= '<a class="pager-item pager-next" href="'.$_current_page.($_page+1).$str_fragment.'" '.$str_target_frame.'>'.$_next.'</a>&nbsp; '."\n";
    }

    // Last page link
	$_last = t('Last Page');

    if ($_page < $_num_page_total) {
        $_buffer .= ' &nbsp;';
        $_buffer .= '<a class="pager-item pager-last" href="'.$_current_page.($_num_page_total).$str_fragment.'" '.$str_target_frame.'>'.$_last.'</a>&nbsp; '."\n";
    }

    $_buffer .= '</div>';

    return $_buffer;
}

?>
