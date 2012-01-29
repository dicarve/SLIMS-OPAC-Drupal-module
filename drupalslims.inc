<?php
/**
 *
 * SLiMS Drupal integration script
 *
 * Copyright (C) 2011 Arie Nugraha (dicarve@yahoo.com)
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

/**
 * Function to show biblio detail
 *
 * @param   integer $int_biblio_id
 * @return  string
 *
 */
function slims_record_detail($int_biblio_id) {
  $SLIMS_BASE_URL = variable_get('slims_base_url', 'http://127.0.0.1/slims');
  $_detail = '';
  $query = '&p=show_detail&id='.$int_biblio_id;
  // get record detail
  $_slims_records = modsXMLsenayan($SLIMS_BASE_URL.'/index.php?inXML=true'.$query, 'uri');

  foreach ($_slims_records['records'] as $record) {
      $_detail .= '<div class="record-detail">'."\n";
      $_detail .= '<h2>'.$record['title'].'</h2>'."\n";
      $_detail .= '<div class="action"><a class="record-back" href="javascript: history.back();">'.t('Back to previous').'</a></div>'."\n";
      $_detail .= '<div class="detail-content image">';
      if ($record['image']) {
          $_detail .= '<img src="'.$SLIMS_BASE_URL.'/lib/phpthumb/phpThumb.php?src=../../images/docs/'.$record['image'].'&w=200" />';
      }
      $_detail .= '</div>'."\n";

      if ($record['digitals']) {
        $_detail .= '<div class="detail-label">'.t('Download(s)').'</div>'."\n";
        $_detail .= '<div class="detail-content digitals"><ul class="digital-list">'."\n";
        foreach ($record['digitals'] as $dig_item) {
                if ($dig_item['mimetype'] == 'application/pdf') {
                    // $_detail .= '<li style="list-style-image: url('.SLIMS_BASE_URL.'/images/labels/ebooks.png)"><strong><a href="#" title="Read the book online" onclick="openHTMLpop(\'index.php?p=fstream&fid='.$dig_item['id'].'&bid='.$record['id'].'\', 830, 500, \''.$dig_item['title'].'\')">'.$dig_item['title'].'</a></strong>';
                    $_detail .= '<li style="list-style-image: url('.$SLIMS_BASE_URL.'/images/labels/ebooks.png)"><strong><a href="'.$SLIMS_BASE_URL.'/repository'.$dig_item['path'].'" title="Download">'.$dig_item['title'].'</a></strong>';
                    $_detail .= '</li>';
                } else if (preg_match('@(video|audio)/.+@i', $dig_item['mimetype'])) {
                    $_detail .= '<li style="list-style-image: url('.$SLIMS_BASE_URL.'images/labels/auvi.png)"><strong><a href="#" title="Click to Play, Listen or View" onclick="openHTMLpop(\'index.php?p=multimediastream&fid='.$reco.'&bid='.$record['id'].'\', 400, 300, \''.$dig_item['title'].'\')">'.$dig_item['title'].'</a></strong>';
                    $_detail .= '<div><i>'.$dig_item['file_desc'].'</i></div>';
                    if (trim($dig_item['url']) != '') { $_detail .= '<div><a href="'.trim($dig_item['url']).'" title="Other Resource Link" target="_blank">Other Resource Link</a></div>'; }
                    $_detail .= '</li>';
                } else if ($dig_item['mimetype'] == 'text/uri-list') {
                    $_detail .= '<li style="list-style-image: url('.$SLIMS_BASE_URL.'/images/labels/url.png)"><strong><a href="'.trim($dig_item['url']).'" title="Click to open link" target="_blank">'.$dig_item['title'].'</a><div>'.$dig_item['file_desc'].'</div></strong></li>';
                } else if (preg_match('@(image).+@i', $dig_item['mimetype'])) {
                    $_detail .= '<li style="list-style-image: url(images/labels/ebooks.png)"><strong><a href="/catalog/index.php?p=fstream&fid='.$dig_item['id'].'&bid='.$record['id'].'" target="_blank">'.$dig_item['title'].'</a></strong>';
                    if (trim($dig_item['url']) != '') { $_detail .= ' [<a href="'.trim($dig_item['url']).'" title="Other Resource related to this file" target="_blank" style="font-size: 90%;">Other Resource Link</a>]'; }
                    $_detail .= '</li>';
                } else {
                    $_detail .= '<li style="list-style-image: url('.$SLIMS_BASE_URL.'/images/labels/ebooks.png)"><strong><a title="Click To View File" href="index.php?p=fstream&fid='.$dig_item['id'].'&bid='.$record['id'].'" target="_blank">'.$dig_item['title'].'</a></strong>';
                    if (trim($dig_item['url']) != '') { $_detail .= ' [<a href="'.trim($dig_item['url']).'" title="Other Resource related to this file" target="_blank" style="font-size: 90%;">Other Resource Link</a>]'; }
                    $_detail .= '</li>';
                }
        }
        $_detail .= '</ul></div>'."\n";
      }


      $_detail .= '<div class="detail-label">'.t('Title').'</div>'."\n";
      $_detail .= '<div class="detail-content title">'.$record['title'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Edition').'</div>'."\n";
      $_detail .= '<div class="detail-content edition">'.$record['edition'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Classification').'</div>'."\n";
      $_detail .= '<div class="detail-content classification">'.$record['classification'].'</div>'."\n";

      // get authors
      $authors = '';
      if ($record['authors']) {
        foreach ($record['authors'] as $author) {
           $authors .= l($author['name'], 'slims/search/'.( urlencode('"'.trim($author['name']).'"') )).' - ';
        }
        // replace last dash char
        $authors = substr_replace($authors, '', -3);
        $_detail .= '<div class="detail-label">'.t('Authors').'</div>'."\n";
        $_detail .= '<div class="detail-content author">'.$authors.'</div>'."\n";
      }

      // get subjects
      $subjects = '';
      if ($record['subjects']) {
        foreach ($record['subjects'] as $subject) {
           $subjects .= l($subject['term'], 'slims/search/'.( urlencode('"'.trim($subject['term']).'"') )).' - ';
        }
        // replace last dash char
        $subjects = substr_replace($subjects, '', -3);
        $_detail .= '<div class="detail-label">'.t('Subjects/Topics').'</div>'."\n";
        $_detail .= '<div class="detail-content subject">'.$subjects.'</div>'."\n";
      }

      $_detail .= '<div class="detail-label">'.t('GMD').'</div>'."\n";
      $_detail .= '<div class="detail-content gmd">'.$record['gmd'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Publisher').'</div>'."\n";
      $_detail .= '<div class="detail-content publisher">'.$record['publisher'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Publish place').'</div>'."\n";
      $_detail .= '<div class="detail-content publish-place">'.$record['publish_place'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Publish year').'</div>'."\n";
      $_detail .= '<div class="detail-content publish-year">'.$record['publish_year'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Language').'</div>'."\n";
      $_detail .= '<div class="detail-content language">'.$record['language']['name'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Collation').'</div>'."\n";
      $_detail .= '<div class="detail-content collation">'.$record['collation'].'</div>'."\n";

      if (isset($record['series_title'])) {
        $_detail .= '<div class="detail-label">'.t('Series title').'</div>'."\n";
        $_detail .= '<div class="detail-content series-title">'.$record['series_title'].'</div>'."\n";
      }

      $_detail .= '<div class="detail-label">'.t('ISBN/ISSN').'</div>'."\n";
      $_detail .= '<div class="detail-content isbn-issn">'.$record['isbn_issn'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Location').'</div>'."\n";
      $_detail .= '<div class="detail-content location">'.$record['location'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Call Number').'</div>'."\n";
      $_detail .= '<div class="detail-content call-number">'.$record['call_number'].'</div>'."\n";

      $_detail .= '<div class="detail-label">'.t('Notes/Abstract').'</div>'."\n";
      $_detail .= '<div class="detail-content notes">'.$record['notes'].'</div>'."\n";

      $_detail .= '<div class="action"><a class="record-back" href="javascript: history.back();">'.t('Back to previous').'</a></div>'."\n";

      // end of record
      $_detail .= '</div>'."\n";
  }
  return $_detail;
}

/**
 * Function to show biblio list
 *
 * @param   string    $str_query
 * @return  string
 */
function slims_biblio($str_keywords = '') {
  $SLIMS_BASE_URL = variable_get('slims_base_url', 'http://127.0.0.1/slims');
  ob_start();
  $_biblio = '';
  $_query = '';
  // get page
  if (isset($_GET['page']) && (integer)$_GET['page'] > 1) {
    $page = (integer)$_GET['page'];
    $_query .= '&page='.$page;
  }

  // keywords
  if (trim($str_keywords) != '') {
    $keywords = trim($str_keywords);
    $_query .= '&keywords='.$keywords.'&search=Search';
  }

  // get record from slims XML
  $_slims_records = modsXMLsenayan($SLIMS_BASE_URL.'/index.php?resultXML=true'.$_query, 'uri');
  if (!$_slims_records['records']) {
    drupal_set_message(t('Sorry no results found from your search. Please try with other keywords'));
    return;
  }

  drupal_set_message(t('Found <strong>%numresult</strong> record(s) from catalog'.( $str_keywords?' with keywords: %keywords':'' ),
    array('%numresult' => $_slims_records['result_num'], '%keywords' => $str_keywords)));
  echo '<div class="slims catalog">'."\n";
  // show records
  foreach ($_slims_records['records'] as $record) {
    echo '<div class="slims-record"'.( $record['image']?' style="background-image: url('.$SLIMS_BASE_URL.'/lib/phpthumb/phpThumb.php?src=../../images/docs/'.$record['image'].'&w=70);"':'' ).'>'."\n";
    echo '<div class="title"><a href="'.url('slims/detail/'.$record['id']).'">'.$record['title'].'</a></div>'."\n";
    // get authors
    $authors = '';
    if ($record['authors']) {
        foreach ($record['authors'] as $author) {
           $authors .= $author['name'].' - ';
        }
        // replace last dash char
        $authors = substr_replace($authors, '', -3);
        echo '<div class="author"><span class="label">'.t('Author').'</span>: '.$authors.'</div>'."\n";
    }

    if ($record['publisher']) {
        echo '<div class="publisher"><span class="label">'.t('Publisher').'</span>: '.$record['publisher'].'</div>'."\n";
    }
    if ($record['publish_year']) {
        echo '<div class="publish_year"><span class="label">'.t('Publish year').'</span>: '.$record['publish_year'].'</div>'."\n";
    }
    if ($record['notes']) {
        echo '<div class="note"><span class="label">'.t('Specific Detail Info').'</span>: '.$record['notes'].'</div>'."\n";
    }

    // end of record
    echo '</div>'."\n";
  }
  echo '</div>'."\n";
  // pager
  echo paging($_slims_records['result_num'], $_slims_records['result_showed'], 10);
  return ob_get_clean();
}


/**
 * Function to show biblio list in simple style
 *
 * @param   string    $str_query
 * @return  string
 */
function slims_biblio_simple($str_keywords = '', $int_num_show = 10, $bool_show_thumb = false) {
    $SLIMS_BASE_URL = variable_get('slims_base_url', 'http://127.0.0.1/slims');
    ob_start();
    $_biblio = '';
    $_query = '';

    // keywords
    if (trim($str_keywords) != '') {
        $keywords = trim($str_keywords);
        $_query .= '&keywords='.$keywords.'&search=Search';
    }

    // get record from slims XML
    $_slims_records = modsXMLsenayan($SLIMS_BASE_URL.'/index.php?resultXML=true'.$_query, 'uri');

    $_r = 0;
    $_thumbnail = '';
    $_biblio .= '<ul class="slims catalog-simple">'."\n";
    // show records
    foreach ($_slims_records['records'] as $record) {
        if ($_r == $int_num_show) {
            break;
        }
        // show thumbnail
        if ($record['image'] && !$_thumbnail && $bool_show_thumb) {
            $_thumbnail = urlencode($record['image']);
        }
        $_biblio .= '<li class="slims-record title"><a href="'.url('slims/detail/'.$record['id']).'">'.$record['title'].'</a></li>'."\n";
        $_r++;
    }
    $_biblio .= '</ul>'."\n";
    if ($_thumbnail && $bool_show_thumb) {
        echo '<div class="catalog-thumb"><img src="'.$SLIMS_BASE_URL.'/lib/phpthumb/phpThumb.php?src=../../images/docs/'.$_thumbnail.'&w=100" /><p>&nbsp;</p></div>';
    }
    echo $_biblio;
    return ob_get_clean();
}
